# CMakeLists.txt en la raíz del proyecto
cmake_minimum_required(VERSION 3.20)
project(MiniDB VERSION 1.0.0 LANGUAGES CXX)

# Configurar C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opciones del proyecto
option(MINIDB_BUILD_TESTS "Build the tests" ON)
option(MINIDB_BUILD_EXAMPLES "Build example applications" OFF)

# Configurar directorios de salida
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Agregar los subdirectorios del código fuente
add_subdirectory(src)

# Configuración de tests si está habilitado
if(MINIDB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Ejemplos (opcional)
if(MINIDB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()