# tests/core/types/CMakeLists.txt
add_executable(minidb_types_tests
        NumberTypeTest.cpp
        NumberTypeTest.hpp
        Varchar2TypeTest.cpp
        Varchar2TypeTest.hpp
        CharTypeTest.cpp
        CharTypeTest.hpp
        NCharTypeTest.cpp
        NCharTypeTest.hpp
        NVarchar2TypeTest.cpp
        NVarchar2TypeTest.hpp
        DateTypeTest.cpp
        DateTypeTest.hpp
        TimestampTypeTest.cpp
        TimestampTypeTest.hpp
)

target_link_libraries(minidb_types_tests
        PRIVATE
        minidb_types
        GTest::gtest_main
)

target_include_directories(minidb_types_tests
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

# Agregar el test a CTest
add_test(NAME minidb_types_tests COMMAND minidb_types_tests)