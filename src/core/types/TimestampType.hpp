#ifndef TIMESTAMP_TYPE_HPP
#define TIMESTAMP_TYPE_HPP

#include "DataType.hpp"
#include "exceptions/DataTypeException.hpp"
#include <chrono>
#include <string>
#include <format>
#include <ctime>
#include <optional>
#include <cmath>

namespace db::types {

    class TimestampType final : public DataType {
    public:
        static constexpr int MAX_YEAR = 9999;
        static constexpr int MIN_YEAR = -4712;  // 4712 BC
        static constexpr int MAX_PRECISION = 9;  // Nanosecond precision
        static constexpr int BASE_SIZE = 7;     // Basic timestamp size in bytes
        static constexpr int MAX_FRACTION_SIZE = 3; // Additional bytes for fraction

        explicit TimestampType(int precision = 6, bool withTimeZone = false, bool isNullable = true)
            : precision_(std::clamp(precision, 0, MAX_PRECISION))
            , withTimeZone_(withTimeZone)
            , nullable_(isNullable) {}

        [[nodiscard]] std::string getName() const override {
            return withTimeZone_ ? "TIMESTAMP WITH TIME ZONE" : "TIMESTAMP";
        }

        [[nodiscard]] size_t getSize() const override {
            size_t size = BASE_SIZE;
            if (precision_ > 0) {
                size += std::min((precision_ + 2) / 3, MAX_FRACTION_SIZE);
            }
            if (withTimeZone_) {
                size += 2; // Extra bytes for timezone
            }
            return size;
        }

        [[nodiscard]] bool isNullable() const override {
            return nullable_;
        }

        [[nodiscard]] std::unique_ptr<DataType> clone() const override {
            return std::make_unique<TimestampType>(precision_, withTimeZone_, nullable_);
        }

        [[nodiscard]] int getPrecision() const noexcept {
            return precision_;
        }

        [[nodiscard]] bool hasTimeZone() const noexcept {
            return withTimeZone_;
        }

        [[nodiscard]] bool isValidTimestamp(
            int year, int month, int day,
            int hour, int minute, int second,
            int fraction = 0,
            std::optional<int> tzOffset = std::nullopt) const noexcept {
            try {
                if (year < MIN_YEAR || year > MAX_YEAR) {
                    return false;
                }

                // Validate basic components
                if (month < 1 || month > 12 || day < 1 || day > 31 ||
                    hour < 0 || hour > 23 || minute < 0 || minute > 59 ||
                    second < 0 || second > 59) {
                    return false;
                }

                // Validate fraction based on precision
                if (fraction < 0) {
                    return false;
                }

                int64_t maxFraction = static_cast<int64_t>(std::pow(10, precision_)) - 1;
                if (fraction > maxFraction) {
                    return false;
                }

                // Validate timezone if present
                if (tzOffset.has_value()) {
                    if (tzOffset.value() < -14 * 60 || tzOffset.value() > 14 * 60) {
                        return false;
                    }
                }

                // Validate timezone presence matches configuration
                if (withTimeZone_ != tzOffset.has_value()) {
                    return false;
                }

                // Use std::chrono for date validation
                std::chrono::year_month_day ymd{
                    std::chrono::year{year},
                    std::chrono::month{static_cast<unsigned>(month)},
                    std::chrono::day{static_cast<unsigned>(day)}
                };

                return ymd.ok();
            } catch (...) {
                return false;
            }
        }

        [[nodiscard]] std::string formatTimestamp(
            int year, int month, int day,
            int hour, int minute, int second,
            int fraction = 0,
            std::optional<int> tzOffset = std::nullopt) const {
            if (!isValidTimestamp(year, month, day, hour, minute, second, fraction, tzOffset)) {
                throw DataTypeException("Invalid timestamp");
            }

            std::string result = std::format("{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}",
                std::abs(year), month, day, hour, minute, second);

            // Add fraction if precision > 0 and fraction > 0
            if (precision_ > 0 && fraction > 0) {
                std::string fractionStr = std::to_string(fraction);
                // Pad with leading zeros if needed
                fractionStr = std::string(precision_ - fractionStr.length(), '0') + fractionStr;
                // Truncate if longer than precision
                fractionStr = fractionStr.substr(0, precision_);
                result += "." + fractionStr;
            }

            // Add BC indicator if needed
            if (year < 0) {
                result += " BC";
            }

            // Add timezone if present
            if (withTimeZone_ && tzOffset.has_value()) {
                int offset = tzOffset.value();
                char sign = offset < 0 ? '-' : '+';
                offset = std::abs(offset);
                int tzHours = offset / 60;
                int tzMinutes = offset % 60;
                result += std::format(" {:c}{:02d}:{:02d}", sign, tzHours, tzMinutes);
            }

            return result;
        }

        [[nodiscard]] static int compareTimestamps(
            int year1, int month1, int day1, int hour1, int minute1, int second1,
            int year2, int month2, int day2, int hour2, int minute2, int second2,
            int fraction1 = 0, int fraction2 = 0,
            std::optional<int> tzOffset1 = std::nullopt,
            std::optional<int> tzOffset2 = std::nullopt) noexcept {
            try {
                using namespace std::chrono;

                // Convert to UTC if timezone is provided
                if (tzOffset1.has_value()) {
                    adjustForTimezone(hour1, minute1, tzOffset1.value());
                }
                if (tzOffset2.has_value()) {
                    adjustForTimezone(hour2, minute2, tzOffset2.value());
                }

                // Compare dates first
                auto tp1 = sys_days{year_month_day{
                    year{year1},
                    month{static_cast<unsigned>(month1)},
                    day{static_cast<unsigned>(day1)}
                }} + hours{hour1} + minutes{minute1} + seconds{second1};

                auto tp2 = sys_days{year_month_day{
                    year{year2},
                    month{static_cast<unsigned>(month2)},
                    day{static_cast<unsigned>(day2)}
                }} + hours{hour2} + minutes{minute2} + seconds{second2};

                if (tp1 < tp2) return -1;
                if (tp1 > tp2) return 1;

                // If dates are equal, compare fractions
                if (fraction1 < fraction2) return -1;
                if (fraction1 > fraction2) return 1;

                return 0;
            } catch (...) {
                return 0;
            }
        }

    private:
        static void adjustForTimezone(int& hour, int& minute, int tzOffset) noexcept {
            minute -= tzOffset;
            while (minute < 0) {
                minute += 60;
                hour--;
            }
            while (minute >= 60) {
                minute -= 60;
                hour++;
            }
            while (hour < 0) hour += 24;
            while (hour >= 24) hour -= 24;
        }

        int precision_;
        bool withTimeZone_;
        bool nullable_;
    };

} // namespace db::types

#endif // TIMESTAMP_TYPE_HPP