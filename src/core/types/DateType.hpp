// src/core/types/DateType.hpp
#ifndef DATE_TYPE_HPP
#define DATE_TYPE_HPP

#include "DataType.hpp"
#include "exceptions/DataTypeException.hpp"
#include <chrono>
#include <string>
#include <format>
#include <ctime>

namespace db::types {

    class DateType final : public DataType {
    public:
        // Oracle DATE almacena: century, year, month, day, hour, minute, second
        static constexpr int MAX_YEAR = 9999;
        static constexpr int MIN_YEAR = -4712;  // 4712 BC
        static constexpr int BYTES_SIZE = 7;    // Tamaño en bytes que usa Oracle

        explicit DateType(bool isNullable = true)
            : nullable(isNullable) {}

        [[nodiscard]] std::string getName() const override {
            return "DATE";
        }

        [[nodiscard]] size_t getSize() const override {
            return BYTES_SIZE;
        }

        [[nodiscard]] bool isNullable() const override {
            return nullable;
        }

        [[nodiscard]] std::unique_ptr<DataType> clone() const override {
            return std::make_unique<DateType>(nullable);
        }

        // Validar una fecha usando std::chrono
        [[nodiscard]] bool isValidDate(int year, int month, int day,
                                     int hour = 0, int minute = 0, int second = 0) const noexcept {
            try {
                if (year < MIN_YEAR || year > MAX_YEAR) {
                    return false;
                }

                // Validar el rango de los componentes
                if (month < 1 || month > 12 || day < 1 || day > 31 ||
                    hour < 0 || hour > 23 || minute < 0 || minute > 59 ||
                    second < 0 || second > 59) {
                    return false;
                }

                // Usar std::chrono para validación adicional
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

        // Formatear fecha según el formato de Oracle
        [[nodiscard]] std::string formatDate(int year, int month, int day,
                                           int hour = 0, int minute = 0, int second = 0) const {
            if (!isValidDate(year, month, day, hour, minute, second)) {
                throw DataTypeException("Invalid date");
            }

            // Si la hora, minuto y segundo son 0, solo mostrar la fecha
            if (hour == 0 && minute == 0 && second == 0) {
                return std::format("{:02d}-{:02d}-{:02d}",
                    std::abs(year), month, day) +
                    (year < 0 ? " BC" : "");
            }

            // Mostrar fecha y hora completa
            return std::format("{:02d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}",
                std::abs(year), month, day, hour, minute, second) +
                (year < 0 ? " BC" : "");
        }

        // Comparar dos fechas
        [[nodiscard]] static int compareDates(
            int year1, int month1, int day1,
            int year2, int month2, int day2,
            int hour1 = 0, int minute1 = 0, int second1 = 0,
            int hour2 = 0, int minute2 = 0, int second2 = 0
        ) noexcept {
            try {
                using namespace std::chrono;

                // Crear puntos en el tiempo para comparación
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
                return 0;
            } catch (...) {
                return 0;  // En caso de error, considerar fechas iguales
            }
        }

    private:
        bool nullable;
    };

} // namespace db::types

#endif // DATE_TYPE_HPP