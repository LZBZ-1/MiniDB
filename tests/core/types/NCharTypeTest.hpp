// tests/core/types/NCharTypeTest.hpp
#ifndef NCHAR_TYPE_TEST_HPP
#define NCHAR_TYPE_TEST_HPP

#include <gtest/gtest.h>
#include "../../../src/core/types/NCharType.hpp"
#include "../../../src/core/types/factories/StringTypeFactory.hpp"

namespace db::types::test {

    class NCharTypeTest : public ::testing::Test {
    protected:
        void SetUp() override {}
        void TearDown() override {}
    };

} // namespace db::types::test


#endif //NCHARTYPETEST_HPP
