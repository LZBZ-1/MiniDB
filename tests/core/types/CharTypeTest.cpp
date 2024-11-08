// tests/core/types/CharTypeTest.cpp
#include "CharTypeTest.hpp"
#include <stdexcept>

namespace db::types::test {

    TEST_F(CharTypeTest, ConstructorShouldInitializeWithValidParameters) {
        CharType charType{true, 10};

        EXPECT_EQ(charType.getLength(), 10);
        EXPECT_TRUE(charType.isNullable());
    }

    TEST_F(CharTypeTest, GetNameShouldReturnCorrectFormat) {
        CharType charType{true, 10};
        EXPECT_EQ(charType.getName(), "CHAR(10)");
    }

    TEST_F(CharTypeTest, ConstructorShouldThrowOnInvalidLength) {
        try {
            CharType invalid{true, CharType::MAX_LENGTH + 1};
            FAIL() << "Expected DataTypeException";
        } catch (const DataTypeException& e) {
            SUCCEED();
        }

        try {
            CharType invalid{true, 0};
            FAIL() << "Expected DataTypeException";
        } catch (const DataTypeException& e) {
            SUCCEED();
        }
    }

    TEST_F(CharTypeTest, FormatValueShouldPadCorrectly) {
        CharType charType{true, 5};

        struct TestCase {
            std::string input;
            std::string expected;
            std::string description;
        };

        const TestCase testCases[] = {
            {"", "     ", "Empty string"},
            {"a", "a    ", "Single character"},
            {"abc", "abc  ", "Partial string"},
            {"abcde", "abcde", "Exact length"},
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(charType.formatValue(tc.input), tc.expected)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(CharTypeTest, FormatValueShouldThrowOnTooLongInput) {
        CharType charType{true, 5};

        EXPECT_THROW({
            charType.formatValue("123456");
        }, DataTypeException);
    }

    TEST_F(CharTypeTest, CompareValuesShouldWorkCorrectly) {
        struct TestCase {
            std::string value1;
            std::string value2;
            bool expectedEqual;
            std::string description;
        };

        const TestCase testCases[] = {
            {"abc  ", "abc", true, "Trailing spaces in first"},
            {"abc", "abc  ", true, "Trailing spaces in second"},
            {"abc  ", "abc  ", true, "Equal with spaces"},
            {"abc", "abc", true, "Equal without spaces"},
            {"abc  ", "abcd ", false, "Different values"},
            {"abc  ", "ab   ", false, "Different with spaces"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(CharType::compareValues(tc.value1, tc.value2), tc.expectedEqual)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(CharTypeTest, CloneShouldCreateExactCopy) {
        CharType original{true, 10};
        auto cloned = original.clone();

        const auto* charClone = dynamic_cast<const CharType*>(cloned.get());
        ASSERT_NE(charClone, nullptr);

        EXPECT_EQ(charClone->getLength(), original.getLength());
        EXPECT_EQ(charClone->isNullable(), original.isNullable());
    }

    TEST_F(CharTypeTest, FactoryMethodsShouldCreateValidInstances) {
        auto defaultChar = StringTypeFactory::createDefaultChar();
        auto customChar = StringTypeFactory::createChar<size_t>(10);

        const auto* defaultType = dynamic_cast<const CharType*>(defaultChar.get());
        const auto* customType = dynamic_cast<const CharType*>(customChar.get());

        ASSERT_NE(defaultType, nullptr);
        ASSERT_NE(customType, nullptr);

        EXPECT_EQ(defaultType->getLength(), 1);
        EXPECT_EQ(customType->getLength(), 10);
    }

} // namespace db::types::test