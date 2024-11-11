// tests/core/types/DateTypeTest.cpp
#include "DateTypeTest.hpp"
#include <stdexcept>

namespace db::types::test {

    TEST_F(DateTypeTest, ConstructorShouldInitializeWithValidParameters) {
        DateType date{true};
        EXPECT_TRUE(date.isNullable());
    }

    TEST_F(DateTypeTest, GetNameShouldReturnCorrectType) {
        DateType date{true};
        EXPECT_EQ(date.getName(), "DATE");
    }

    TEST_F(DateTypeTest, ShouldValidateCorrectDates) {
        DateType date{true};

        struct TestCase {
            int year;
            int month;
            int day;
            int hour;
            int minute;
            int second;
            bool expectedValid;
            std::string description;
        };

        const TestCase testCases[] = {
            {2024, 1, 1, 0, 0, 0, true, "Valid date without time"},
            {2024, 2, 29, 0, 0, 0, true, "Valid leap year date"},
            {2023, 2, 29, 0, 0, 0, false, "Invalid date - not a leap year"},
            {-4712, 1, 1, 0, 0, 0, true, "Minimum valid year"},
            {9999, 12, 31, 23, 59, 59, true, "Maximum valid date and time"},
            {10000, 1, 1, 0, 0, 0, false, "Year too large"},
            {-4713, 1, 1, 0, 0, 0, false, "Year too small"},
            {2024, 13, 1, 0, 0, 0, false, "Invalid month"},
            {2024, 4, 31, 0, 0, 0, false, "Invalid day for April"},
            {2024, 1, 1, 24, 0, 0, false, "Invalid hour"},
            {2024, 1, 1, 0, 60, 0, false, "Invalid minute"},
            {2024, 1, 1, 0, 0, 60, false, "Invalid second"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(date.isValidDate(tc.year, tc.month, tc.day,
                                     tc.hour, tc.minute, tc.second),
                     tc.expectedValid)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(DateTypeTest, ShouldFormatDatesCorrectly) {
        DateType date{true};

        struct TestCase {
            int year;
            int month;
            int day;
            int hour;
            int minute;
            int second;
            std::string expectedFormat;
            std::string description;
        };

        const TestCase testCases[] = {
            {2024, 1, 1, 0, 0, 0, "2024-01-01", "Date only"},
            {2024, 12, 31, 23, 59, 59, "2024-12-31 23:59:59", "Date with time"},
            {-4712, 1, 1, 0, 0, 0, "4712-01-01 BC", "BC date"},
            {-1, 1, 1, 12, 0, 0, "01-01-01 12:00:00 BC", "BC date with time"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(date.formatDate(tc.year, tc.month, tc.day,
                                    tc.hour, tc.minute, tc.second),
                     tc.expectedFormat)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(DateTypeTest, ShouldCompareCorrectly) {
        struct TestCase {
            struct Date {
                int year, month, day, hour, minute, second;
            };
            Date date1;
            Date date2;
            int expectedResult;
            std::string description;
        };

        const TestCase testCases[] = {
            {
                {2024, 1, 1, 0, 0, 0},    // date1
                {2024, 1, 1, 0, 0, 0},    // date2
                0,                         // expectedResult
                "Equal dates"              // description
            },
            {
                {2024, 1, 1, 0, 0, 0},
                {2024, 1, 2, 0, 0, 0},
                -1,
                "Earlier date"
            },
            {
                {2024, 1, 2, 0, 0, 0},
                {2024, 1, 1, 0, 0, 0},
                1,
                "Later date"
            },
            {
                {2024, 1, 1, 12, 0, 0},
                {2024, 1, 1, 13, 0, 0},
                -1,
                "Earlier time"
            },
            {
                {-4712, 1, 1, 0, 0, 0},
                {9999, 12, 31, 23, 59, 59},
                -1,
                "Minimum vs maximum date"
            }
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(DateType::compareDates(
                tc.date1.year, tc.date1.month, tc.date1.day,
                tc.date2.year, tc.date2.month, tc.date2.day,
                tc.date1.hour, tc.date1.minute, tc.date1.second,
                tc.date2.hour, tc.date2.minute, tc.date2.second
            ), tc.expectedResult) << "Failed for " << tc.description;
        }
    }

    TEST_F(DateTypeTest, FactoryMethodShouldCreateValidInstance) {
        auto date = DateTypeFactory::createDate();
        EXPECT_NE(date, nullptr);
        EXPECT_EQ(date->getName(), "DATE");
    }

} // namespace db::types::test