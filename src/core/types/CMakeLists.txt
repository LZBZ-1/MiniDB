# src/core/types/CMakeLists.txt
add_library(minidb_types
        # Headers
        DataType.hpp
        NumericType.hpp
        NumberType.hpp
        Varchar2Type.hpp
        CharType.hpp
        NCharType.hpp
        NVarchar2Type.hpp
        DataType.hpp
        TimestampType.hpp
        exceptions/DataTypeException.hpp
        factories/NumericTypeFactory.hpp
        factories/StringTypeFactory.hpp
        factories/DateTimeTypeFactory.hpp

        # Implementations
        NumberType.cpp
)

target_link_libraries(minidb_types
        PUBLIC
        minidb_core
)

target_include_directories(minidb_types
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
)

# Habilitar las características de C++20
target_compile_features(minidb_types PUBLIC cxx_std_20)