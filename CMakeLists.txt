cmake_minimum_required(VERSION 3.20)
project(CPP_Libmem LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(libmem STATIC
        ${SRC_DIR}/cpp_libmem.cpp
        ${INC_DIR}/cpp_libmem.hpp
        ${SRC_DIR}/process.cpp
        ${INC_DIR}/process.hpp
        src/thread.cpp
        include/thread.hpp
        src/module.cpp
        include/module.hpp
        internal/winutils/winutils.cpp
        internal/winutils/winutils.hpp

)

target_include_directories(libmem PUBLIC ${INC_DIR})

add_executable(tests
        tests/Tests_Process.cpp
        )

target_link_libraries(tests libmem)

