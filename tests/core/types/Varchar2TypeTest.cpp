// tests/core/types/Varchar2TypeTest.cpp
#include "Varchar2TypeTest.hpp"
#include <stdexcept>

namespace db::types::test {

    TEST_F(Varchar2TypeTest, ConstructorShouldInitializeWithValidParameters) {
        Varchar2Type varchar{true, 100};

        EXPECT_EQ(varchar.getMaxLength(), 100);
        EXPECT_TRUE(varchar.isNullable());
    }

    TEST_F(Varchar2TypeTest, GetNameShouldReturnCorrectFormat) {
        Varchar2Type varchar{true, 200};
        EXPECT_EQ(varchar.getName(), "VARCHAR2(200)");
    }

    TEST_F(Varchar2TypeTest, ConstructorShouldThrowOnInvalidLength) {
        try {
            Varchar2Type invalid{true, Varchar2Type::MAX_LENGTH + 1};
            FAIL() << "Expected DataTypeException";
        } catch (const DataTypeException& e) {
            SUCCEED();
        }

        try {
            Varchar2Type invalid{true, 0};
            FAIL() << "Expected DataTypeException";
        } catch (const DataTypeException& e) {
            SUCCEED();
        }
    }

    TEST_F(Varchar2TypeTest, GetSizeShouldReturnMaxLength) {
        const size_t testSizes[] = {10, 100, 1000, 4000};

        for (auto size : testSizes) {
            Varchar2Type varchar{true, size};
            EXPECT_EQ(varchar.getSize(), size)
                << "Failed for size=" << size;
        }
    }

    TEST_F(Varchar2TypeTest, CloneShouldCreateExactCopy) {
        Varchar2Type original{true, 150};
        auto cloned = original.clone();

        const auto* varchar2Clone = dynamic_cast<const Varchar2Type*>(cloned.get());
        ASSERT_NE(varchar2Clone, nullptr);

        EXPECT_EQ(varchar2Clone->getMaxLength(), original.getMaxLength());
        EXPECT_EQ(varchar2Clone->isNullable(), original.isNullable());
    }

    TEST_F(Varchar2TypeTest, IsValidValueShouldWorkCorrectly) {
        Varchar2Type varchar{true, 5};

        struct TestCase {
            std::string value;
            bool expectedResult;
            std::string description;
        };

        const TestCase testCases[] = {
            {"", true, "Empty string"},
            {"abc", true, "String within limit"},
            {"12345", true, "String at limit"},
            {"123456", false, "String exceeding limit"},
            {"こんにちは", false, "Multibyte string exceeding limit"}
        };

        for (const auto& tc : testCases) {
            EXPECT_EQ(varchar.isValidValue(tc.value), tc.expectedResult)
                << "Failed for " << tc.description
                << " (value=\"" << tc.value << "\")";
        }
    }

    TEST_F(Varchar2TypeTest, FactoryMethodsShouldCreateValidInstances) {
        auto defaultVarchar = StringTypeFactory::createDefaultVarchar2();
        auto customVarchar = StringTypeFactory::createVarchar2<size_t>(100, true);

        EXPECT_EQ(defaultVarchar->getName(), "VARCHAR2(4000)");
        EXPECT_EQ(customVarchar->getName(), "VARCHAR2(100)");

        const auto* defaultType = dynamic_cast<const Varchar2Type*>(defaultVarchar.get());
        const auto* customType = dynamic_cast<const Varchar2Type*>(customVarchar.get());

        ASSERT_NE(defaultType, nullptr);
        ASSERT_NE(customType, nullptr);

        EXPECT_EQ(defaultType->getMaxLength(), Varchar2Type::MAX_LENGTH);
        EXPECT_EQ(customType->getMaxLength(), 100);
    }

} // namespace db::types::test