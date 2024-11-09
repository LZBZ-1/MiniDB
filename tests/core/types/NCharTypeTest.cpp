// tests/core/types/NCharTypeTest.cpp
#include "NCharTypeTest.hpp"
#include <stdexcept>

namespace db::types::test {

    TEST_F(NCharTypeTest, ConstructorShouldInitializeWithValidParameters) {
        NCharType ncharType{true, 10};

        EXPECT_EQ(ncharType.getLength(), 10);
        EXPECT_TRUE(ncharType.isNullable());
    }

    TEST_F(NCharTypeTest, GetNameShouldReturnCorrectFormat) {
        NCharType ncharType{true, 10};
        EXPECT_EQ(ncharType.getName(), "NCHAR(10)");
    }

    TEST_F(NCharTypeTest, ShouldCalculateCorrectSize) {
        NCharType ncharType{true, 10};
        EXPECT_EQ(ncharType.getSize(), 20);  // 10 caracteres * 2 bytes
    }

    TEST_F(NCharTypeTest, ShouldHandleUnicodeCharacterCounting) {
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
            {"🌟⭐✨", 3, "Emojis"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(NCharType::getUnicodeLength(tc.value), tc.expectedLength)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(NCharTypeTest, FormatValueShouldPadCorrectly) {
        NCharType ncharType{true, 5};

        struct TestCase {
            std::string input;
            size_t expectedLength;
            std::string description;
        };

        const TestCase testCases[] = {
            {"", 5, "Empty string"},
            {"あ", 5, "Single Unicode character"},
            {"あいう", 5, "Partial Unicode string"},
            {"あいうえお", 5, "Exact length Unicode"}
        };

        for (const auto& tc : testCases) {
            std::string formatted = ncharType.formatValue(tc.input);
            EXPECT_EQ(NCharType::getUnicodeLength(formatted), tc.expectedLength)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(NCharTypeTest, ShouldValidateValueLength) {
        NCharType ncharType{true, 5};

        struct TestCase {
            std::string value;
            bool expectedResult;
            std::string description;
        };

        const TestCase testCases[] = {
            {"Hello", true, "5 ASCII chars - exactly at limit"},
            {"こんに", true, "3 Japanese chars - under limit"},
            {"こんにちは", true, "5 Japanese chars - exactly at limit"},
            {"Hello World", false, "11 ASCII chars - over limit"},
            {"こんにちはあ", false, "6 Japanese chars - over limit"},
            {"Hello世界", false, "7 chars mixed - over limit"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(ncharType.isValidValue(tc.value), tc.expectedResult)
                << "Failed for " << tc.description
                << " (value=" << tc.value << ")"
                << " [Length=" << NCharType::getUnicodeLength(tc.value) << "]";
        }
    }

    TEST_F(NCharTypeTest, CompareValuesShouldWorkCorrectly) {
        struct TestCase {
            std::string value1;
            std::string value2;
            bool expectedEqual;
            std::string description;
        };

        const TestCase testCases[] = {
            {"こんに  ", "こんに", true, "Unicode with trailing spaces"},
            {"Hello", "Hello  ", true, "ASCII with trailing spaces"},
            {"あいうえお", "あいうえお", true, "Equal Unicode strings"},
            {"こんに", "さよな", false, "Different Unicode strings"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(NCharType::compareValues(tc.value1, tc.value2), tc.expectedEqual)
                << "Failed for " << tc.description;
        }
    }

    TEST_F(NCharTypeTest, FactoryMethodsShouldCreateValidInstances) {
        auto defaultNChar = StringTypeFactory::createDefaultNChar();
        auto customNChar = StringTypeFactory::createNChar<size_t>(10);

        const auto* defaultType = dynamic_cast<const NCharType*>(defaultNChar.get());
        const auto* customType = dynamic_cast<const NCharType*>(customNChar.get());

        ASSERT_NE(defaultType, nullptr);
        ASSERT_NE(customType, nullptr);

        EXPECT_EQ(defaultType->getLength(), 1);
        EXPECT_EQ(customType->getLength(), 10);
    }

} // namespace db::types::test