// tests/core/types/NVarchar2TypeTest.hpp
#ifndef NVARCHAR2_TYPE_TEST_HPP
#define NVARCHAR2_TYPE_TEST_HPP

#include <gtest/gtest.h>
#include "../../../src/core/types/NVarchar2Type.hpp"
#include "../../../src/core/types/factories/StringTypeFactory.hpp"

namespace db::types::test {

    class NVarchar2TypeTest : public ::testing::Test {
    protected:
        void SetUp() override {}
        void TearDown() override {}
    };

} // namespace db::types::test

#endif //NVARCHAR2_TYPE_TEST_HPP
