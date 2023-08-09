#include "scanner.h"

#include <gtest/gtest.h>

#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "token.h"

std::vector<std::string> readExpected(std::string path) {
  std::vector<std::string> lines;
  std::ifstream file(path);
  std::string line;
  if (!file) {
    std::cerr << "Failed to open file " << path << ": " << std::strerror(errno)
              << "\n";
    std::exit(74);
  }
  while (getline(file, line)) {
    lines.push_back(line);
  }
  file.close();
  return lines;
}

std::vector<Token> load(std::string path) {
  std::ifstream file{path.data(),
                     std::ios::in | std::ios::binary | std::ios::ate};
  if (!file) {
    std::cerr << "Failed to open file " << path << ": " << std::strerror(errno)
              << "\n";
    std::exit(74);
  }
  std::string contents;
  contents.resize(file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(contents.data(), contents.size());
  Scanner scanner{contents};
  std::vector<Token> tokens = scanner.scanTokens();
  return tokens;
}

TEST(ScannerTests, Test1) {
  auto expected = readExpected("./test-lexing1.lox.expected");
  auto tokens = load("./test-lexing1.lox");
  auto expectedIter = expected.begin();
  for (const Token &token : tokens) {
    ASSERT_EQ(token.toString(), *expectedIter);
    ++expectedIter;
  }
}

TEST(ScannerTests, Test2) {
  auto expected = readExpected("./test-lexing2.lox.expected");
  auto tokens = load("./test-lexing2.lox");
  auto expectedIter = expected.begin();
  for (const Token &token : tokens) {
    ASSERT_EQ(token.toString(), *expectedIter);
    ++expectedIter;
  }
}
