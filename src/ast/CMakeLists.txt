project(ast)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)

add_executable(gen_ast generate_ast.cc)

add_executable(ast_printer expr.h ast_printer.h ast_printer_dirver.cc)