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
        src/ast/expr.h
        src/ast/ast_printer.h
        src/parser/parser.h
        src/parser/parser.cc
        src/types/token.h
        src/types/token.cc
        src/error/error.h
        src/lox.cc
        src/ast/interpreter.h
        src/ast/interpreter.cc
)

add_subdirectory(src/scanner)
add_subdirectory(src/ast)
