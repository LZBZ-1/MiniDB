// tests/core/types/CharTypeTest.hpp
#ifndef CHAR_TYPE_TEST_HPP
#define CHAR_TYPE_TEST_HPP

#include <gtest/gtest.h>
#include "../../../src/core/types/CharType.hpp"
#include "../../../src/core/types/factories/StringTypeFactory.hpp"

namespace db::types::test {

    class CharTypeTest : public ::testing::Test {
    protected:
        void SetUp() override {}
        void TearDown() override {}
    };

} // namespace db::types::test

#endif // CHAR_TYPE_TEST_HPP