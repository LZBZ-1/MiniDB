// tests/core/types/NVarchar2TypeTest.cpp
#include "NVarchar2TypeTest.hpp"
#include <stdexcept>

namespace db::types::test {

    TEST_F(NVarchar2TypeTest, ConstructorShouldInitializeWithValidParameters) {
        NVarchar2Type nvarchar{true, 100};

        EXPECT_EQ(nvarchar.getMaxLength(), 100);
        EXPECT_TRUE(nvarchar.isNullable());
    }

    TEST_F(NVarchar2TypeTest, GetNameShouldReturnCorrectFormat) {
        NVarchar2Type nvarchar{true, 100};
        EXPECT_EQ(nvarchar.getName(), "NVARCHAR2(100)");
    }

    TEST_F(NVarchar2TypeTest, ShouldCalculateCorrectSize) {
        NVarchar2Type nvarchar{true, 10};
        EXPECT_EQ(nvarchar.getSize(), 20);  // 10 caracteres * 2 bytes
    }

    TEST_F(NVarchar2TypeTest, ShouldHandleUnicodeCharacterCounting) {
        struct TestCase {
            std::string value;
            size_t expectedLength;
            std::string description;
        };

        const TestCase testCases[] = {
            {"Hello", 5, "ASCII string"},
            {"こんにちは", 5, "Japanese characters"},
            {"αβγδε", 5, "Greek characters"},
            {"Hello こんにちは", 11, "Mixed characters"},
            {"🌟⭐✨", 3, "Emojis"},
            {"", 0, "Empty string"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(NVarchar2Type::getUnicodeLength(tc.value), tc.expectedLength)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(NVarchar2TypeTest, FormatValueShouldNotPad) {
        NVarchar2Type nvarchar{true, 5};

        struct TestCase {
            std::string input;
            std::string expectedOutput;
            std::string description;
        };

        const TestCase testCases[] = {
            {"", "", "Empty string"},
            {"あ", "あ", "Single Unicode character"},
            {"あいう", "あいう", "Multiple Unicode characters"},
            {"Hello", "Hello", "ASCII string"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(nvarchar.formatValue(tc.input), tc.expectedOutput)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(NVarchar2TypeTest, ShouldValidateValueLength) {
        NVarchar2Type nvarchar{true, 5};

        struct TestCase {
            std::string value;
            bool expectedResult;
            std::string description;
        };

        const TestCase testCases[] = {
            {"", true, "Empty string"},
            {"Hello", true, "5 ASCII chars - at limit"},
            {"こんに", true, "3 Japanese chars - under limit"},
            {"こんにちは", true, "5 Japanese chars - at limit"},
            {"Hello World", false, "11 ASCII chars - over limit"},
            {"こんにちはあ", false, "6 Japanese chars - over limit"},
            {"Hello世界", false, "7 chars mixed - over limit"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(nvarchar.isValidValue(tc.value), tc.expectedResult)
                << "Failed for " << tc.description
                << " (value=" << tc.value << ")"
                << " [Length=" << NVarchar2Type::getUnicodeLength(tc.value) << "]";
        }
    }

    TEST_F(NVarchar2TypeTest, CompareValuesShouldBeExact) {
        struct TestCase {
            std::string value1;
            std::string value2;
            bool expectedEqual;
            std::string description;
        };

        const TestCase testCases[] = {
            {"Hello  ", "Hello", false, "Different due to trailing spaces"},
            {"こんに", "こんに  ", false, "Different due to trailing spaces with Unicode"},
            {"Hello", "Hello", true, "Exact match ASCII"},
            {"こんにちは", "こんにちは", true, "Exact match Unicode"},
            {"Hello世界", "Hello世界", true, "Exact match mixed"},
            {"", "", true, "Empty strings"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(NVarchar2Type::compareValues(tc.value1, tc.value2), tc.expectedEqual)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(NVarchar2TypeTest, FactoryMethodsShouldCreateValidInstances) {
        auto defaultNVarchar = StringTypeFactory::createDefaultNVarchar2();
        auto customNVarchar = StringTypeFactory::createNVarchar2<size_t>(100);

        const auto* defaultType = dynamic_cast<const NVarchar2Type*>(defaultNVarchar.get());
        const auto* customType = dynamic_cast<const NVarchar2Type*>(customNVarchar.get());

        ASSERT_NE(defaultType, nullptr);
        ASSERT_NE(customType, nullptr);

        EXPECT_EQ(defaultType->getMaxLength(), NVarchar2Type::MAX_LENGTH);
        EXPECT_EQ(customType->getMaxLength(), 100);
    }

} // namespace db::types::test