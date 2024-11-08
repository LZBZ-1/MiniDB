#ifndef VARCHAR2_TYPE_TEST_HPP
#define VARCHAR2_TYPE_TEST_HPP

#include <gtest/gtest.h>
#include "../../../src/core/types/Varchar2Type.hpp"
#include "../../../src/core/types/factories/StringTypeFactory.hpp"

namespace db::types::test {

    class Varchar2TypeTest : public ::testing::Test {
    protected:
        void SetUp() override {}
        void TearDown() override {}
    };

} // namespace db::types::test

#endif // VARCHAR2_TYPE_TEST_HPP