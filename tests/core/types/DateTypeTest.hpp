// tests/core/types/DateTypeTest.hpp
#ifndef DATE_TYPE_TEST_HPP
#define DATE_TYPE_TEST_HPP

#include <gtest/gtest.h>
#include "../../../src/core/types/DateType.hpp"
#include "../../../src/core/types/factories/DateTypeFactory.hpp"

namespace db::types::test {

    class DateTypeTest : public ::testing::Test {
    protected:
        void SetUp() override {}
        void TearDown() override {}
    };

} // namespace db::types::test

#endif // DATE_TYPE_TEST_HPP