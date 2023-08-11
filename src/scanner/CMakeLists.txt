project(ScannerTest)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/testfiles)

add_executable(test_scanner scanner.h scanner.cc ../types/token.h ../types/token.cc scanner_test.cc)

target_link_libraries(test_scanner
  GTest::GTest
  GTest::Main
)