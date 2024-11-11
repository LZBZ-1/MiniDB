#include "TimestampTypeTest.hpp"
#include <stdexcept>

namespace db::types::test {

    TEST_F(TimestampTypeTest, ConstructorShouldInitializeWithValidParameters) {
        TimestampType ts1{6, false, true};
        EXPECT_TRUE(ts1.isNullable());
        EXPECT_EQ(ts1.getPrecision(), 6);
        EXPECT_FALSE(ts1.hasTimeZone());

        TimestampType ts2{9, true, false};
        EXPECT_FALSE(ts2.isNullable());
        EXPECT_EQ(ts2.getPrecision(), 9);
        EXPECT_TRUE(ts2.hasTimeZone());

        // Test precision clamping
        TimestampType ts3{12, false, true};  // Should clamp to 9
        EXPECT_EQ(ts3.getPrecision(), 9);
    }

    TEST_F(TimestampTypeTest, GetNameShouldReturnCorrectType) {
        TimestampType ts1{6, false};
        EXPECT_EQ(ts1.getName(), "TIMESTAMP");

        TimestampType ts2{6, true};
        EXPECT_EQ(ts2.getName(), "TIMESTAMP WITH TIME ZONE");
    }

    TEST_F(TimestampTypeTest, GetSizeShouldReturnCorrectSize) {
        // Base size tests
        TimestampType ts1{0, false};
        EXPECT_EQ(ts1.getSize(), 7);  // Base size without fraction

        // Fraction size tests
        TimestampType ts2{3, false};
        EXPECT_EQ(ts2.getSize(), 8);  // Base + 1 fraction byte

        TimestampType ts3{6, false};
        EXPECT_EQ(ts3.getSize(), 9);  // Base + 2 fraction bytes

        TimestampType ts4{9, false};
        EXPECT_EQ(ts4.getSize(), 10); // Base + 3 fraction bytes

        // Timezone tests
        TimestampType ts5{6, true};
        EXPECT_EQ(ts5.getSize(), 11); // Base + 2 fraction bytes + 2 timezone bytes
    }

    TEST_F(TimestampTypeTest, ShouldValidateCorrectTimestamps) {
        TimestampType ts{9, true};  // Use 9 digits precision for maximum fraction test

        struct TestCase {
            int year;
            int month;
            int day;
            int hour;
            int minute;
            int second;
            int fraction;
            std::optional<int> tzOffset;
            bool expectedValid;
            std::string description;
        };

        const TestCase testCases[] = {
            {2024, 1, 1, 0, 0, 0, 0, 120,
             true, "Basic valid timestamp"},

            {2024, 2, 29, 12, 30, 45, 123456, 120,
             true, "Leap year with fraction and timezone"},

            {2023, 2, 29, 0, 0, 0, 0, 120,
             false, "Invalid date - not leap year"},

            {-4712, 1, 1, 0, 0, 0, 0, 0,
             true, "Minimum valid year"},

            {9999, 12, 31, 23, 59, 59, 999999999, 120,
             true, "Maximum valid timestamp"},

            {10000, 1, 1, 0, 0, 0, 0, 120,
             false, "Year too large"},

            {2024, 13, 1, 0, 0, 0, 0, 120,
             false, "Invalid month"},

            {2024, 1, 1, 24, 0, 0, 0, 120,
             false, "Invalid hour"},

            {2024, 1, 1, 0, 60, 0, 0, 120,
             false, "Invalid minute"},

            {2024, 1, 1, 0, 0, 60, 0, 120,
             false, "Invalid second"},

            {2024, 1, 1, 0, 0, 0, 1000000000, 120,
             false, "Fraction exceeds precision"},

            {2024, 1, 1, 0, 0, 0, 0, 900,
             false, "Invalid timezone offset (>14 hours)"},

            {2024, 1, 1, 0, 0, 0, 0, -900,
             false, "Invalid timezone offset (<-14 hours)"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(ts.isValidTimestamp(tc.year, tc.month, tc.day,
                                        tc.hour, tc.minute, tc.second,
                                        tc.fraction, tc.tzOffset),
                     tc.expectedValid)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(TimestampTypeTest, ShouldFormatTimestampsCorrectly) {
        struct TestCase {
            int precision;
            bool withTZ;
            int year;
            int month;
            int day;
            int hour;
            int minute;
            int second;
            int fraction;
            std::optional<int> tzOffset;
            std::string expectedFormat;
            std::string description;
        };

        const TestCase testCases[] = {
            // Basic timestamp without timezone
            {
                6, false,
                2024, 1, 1, 0, 0, 0, 0, std::nullopt,
                "2024-01-01 00:00:00",
                "Basic timestamp without fraction"
            },

            // Timestamp with 6-digit precision without timezone
            {
                6, false,
                2024, 12, 31, 23, 59, 59, 123456, std::nullopt,
                "2024-12-31 23:59:59.123456",
                "Timestamp with 6 decimal places"
            },

            // Timestamp with 3-digit precision without timezone
            {
                3, false,
                2024, 6, 15, 8, 30, 0, 123, std::nullopt,
                "2024-06-15 08:30:00.123",
                "Timestamp with 3 decimal places"
            },

            // Timestamp with timezone
            {
                6, true,
                2024, 12, 31, 23, 59, 59, 123456, 120,
                "2024-12-31 23:59:59.123456 +02:00",
                "Timestamp with timezone positive offset"
            },

            // BC date with timezone
            {
                6, true,
                -4712, 1, 1, 12, 0, 0, 0, -330,
                "4712-01-01 12:00:00 BC -05:30",
                "BC date with negative timezone offset"
            },

            // Zero timezone offset
            {
                0, true,
                2024, 6, 15, 8, 30, 0, 0, 0,
                "2024-06-15 08:30:00 +00:00",
                "Timestamp with zero timezone offset"
            },

            // Maximum precision timestamp
            {
                9, false,
                2024, 3, 15, 14, 30, 45, 123456789, std::nullopt,
                "2024-03-15 14:30:45.123456789",
                "Timestamp with maximum precision"
            }
        };

        for (const auto& tc : testCases) {
            TimestampType ts{tc.precision, tc.withTZ};
            EXPECT_EQ(ts.formatTimestamp(tc.year, tc.month, tc.day,
                                       tc.hour, tc.minute, tc.second,
                                       tc.fraction, tc.tzOffset),
                     tc.expectedFormat)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(TimestampTypeTest, ShouldCompareTimestampsCorrectly) {
        struct TestCase {
            struct TimeData {
                TimeData(int y, int mo, int d, int h, int mi, int s, int f, std::optional<int> tz)
                    : year(y), month(mo), day(d), hour(h), minute(mi), second(s),
                      fraction(f), tzOffset(tz) {}
                int year, month, day, hour, minute, second;
                int fraction;
                std::optional<int> tzOffset;
            };
            TestCase(TimeData t1_, TimeData t2_, int result, std::string desc)
                : t1(t1_), t2(t2_), expectedResult(result), description(desc) {}

            TimeData t1;
            TimeData t2;
            int expectedResult;
            std::string description;
        };

        const TestCase testCases[] = {
            TestCase(
                {2024, 1, 1, 0, 0, 0, 0, std::nullopt},
                {2024, 1, 1, 0, 0, 0, 0, std::nullopt},
                0,
                "Equal timestamps"
            ),
            TestCase(
                {2024, 1, 1, 0, 0, 0, 0, std::nullopt},
                {2024, 1, 1, 0, 0, 1, 0, std::nullopt},
                -1,
                "Earlier second"
            ),
            TestCase(
                {2024, 1, 1, 0, 0, 0, 100, std::nullopt},
                {2024, 1, 1, 0, 0, 0, 0, std::nullopt},
                1,
                "Later fraction"
            ),
            TestCase(
                {2024, 1, 1, 12, 0, 0, 0, 120},  // +02:00
                {2024, 1, 1, 10, 0, 0, 0, 0},    // UTC
                0,
                "Equal times in different timezones"
            ),
            TestCase(
                {-4712, 1, 1, 0, 0, 0, 0, std::nullopt},
                {9999, 12, 31, 23, 59, 59, 999999999, std::nullopt},
                -1,
                "Minimum vs maximum timestamp"
            )
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(TimestampType::compareTimestamps(
                tc.t1.year, tc.t1.month, tc.t1.day,
                tc.t1.hour, tc.t1.minute, tc.t1.second,
                tc.t2.year, tc.t2.month, tc.t2.day,
                tc.t2.hour, tc.t2.minute, tc.t2.second,
                tc.t1.fraction, tc.t2.fraction,
                tc.t1.tzOffset, tc.t2.tzOffset),
                tc.expectedResult)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(TimestampTypeTest, FactoryMethodShouldCreateValidInstances) {
        auto ts1 = DateTimeTypeFactory::createTimestamp();
        EXPECT_NE(ts1, nullptr);
        EXPECT_EQ(ts1->getName(), "TIMESTAMP");

        auto ts2 = DateTimeTypeFactory::createTimestampTZ();
        EXPECT_NE(ts2, nullptr);
        EXPECT_EQ(ts2->getName(), "TIMESTAMP WITH TIME ZONE");
    }

    TEST_F(TimestampTypeTest, ShouldThrowOnInvalidTimestamp) {
        TimestampType ts{6, true};
        EXPECT_THROW(
            ts.formatTimestamp(10000, 1, 1, 0, 0, 0),
            DataTypeException
        );
        EXPECT_THROW(
            ts.formatTimestamp(2024, 13, 1, 0, 0, 0),
            DataTypeException
        );
        EXPECT_THROW(
            ts.formatTimestamp(2024, 1, 1, 24, 0, 0),
            DataTypeException
        );
    }

} // namespace db::types::test