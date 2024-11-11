#ifndef TIMESTAMP_TYPE_TEST_HPP
#define TIMESTAMP_TYPE_TEST_HPP

#include <gtest/gtest.h>
#include "../../../src/core/types/TimestampType.hpp"
#include "../../../src/core/types/factories/DateTimeTypeFactory.hpp"

namespace db::types::test {

    class TimestampTypeTest : public ::testing::Test {
    protected:
        void SetUp() override {}
        void TearDown() override {}
    };

} // namespace db::types::test

#endif // TIMESTAMP_TYPE_TEST_HPP