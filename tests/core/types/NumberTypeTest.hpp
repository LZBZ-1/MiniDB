// tests/core/types/NumberTypeTest.hpp
#ifndef NUMBER_TYPE_TEST_HPP
#define NUMBER_TYPE_TEST_HPP

#include <gtest/gtest.h>
#include "../../../src/core/types/NumberType.hpp"
#include "../../../src/core/types/factories/NumericTypeFactory.hpp"
#include "../../../src/core/types/exceptions/DataTypeException.hpp"

namespace db::types::test {

    class NumberTypeTest : public ::testing::Test {
    protected:
        void SetUp() override {}
        void TearDown() override {}
    };

} // namespace db::types::test

#endif // NUMBER_TYPE_TEST_HPP