cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17) # Compile as C++17.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Require C++17 support.
set(BUILD_SHARED_LIBS ON) # We expect external libraries to be linked statically.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(lox
        DESCRIPTION "lox"
        LANGUAGES C CXX)

# gest
find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS})

add_executable(lox
        src/scanner/scanner.h
        src/scanner/scanner.cc
        src/treewalk/expr.h
        src/treewalk/parser.h
        src/treewalk/parser.cc
        src/token/token.h
        src/token/token.cc
        src/utils/error.h
        src/lox.cc
        src/treewalk/interpreter.h
        src/treewalk/interpreter.cc
        src/treewalk/RuntimeError.h
)

add_subdirectory(src/scanner)
add_subdirectory(src/utils)
