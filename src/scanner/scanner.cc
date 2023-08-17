#include "scanner.h"

#include <ctime>
#include <vector>

#include "../types/token.h"

std::vector<Token> Scanner::scanTokens() {
  while (!isAtEnd()) {
    start_ = current_;
    scanToken();
  }
  tokens_.emplace_back(END_OF_FILE, "", nullptr, line_);
  return tokens_;
}

bool Scanner::isAtEnd() { return current_ >= source_.size(); }

void Scanner::scanToken() {
  char c = advance();
  switch (c) {
    case '(':
      addToken(LEFT_PAREN);
      break;
    case ')':
      addToken(RIGHT_PAREN);
      break;
    case '{':
      addToken(LEFT_BRACE);
      break;
    case '}':
      addToken(RIGHT_BRACE);
      break;
    case ',':
      addToken(COMMA);
      break;
    case '.':
      addToken(DOT);
      break;
    case '-':
      addToken(MINUS);
      break;
    case '+':
      addToken(PLUS);
      break;
    case ';':
      addToken(SEMICOLON);
      break;
    case '*':
      addToken(STAR);
      break;
    case '!':
      addToken(match('=') ? BANG_EQUAL : BANG);
      break;
    case '=':
      addToken(match('=') ? EQUAL_EQUAL : EQUAL);
      break;
    case '<':
      addToken(match('=') ? LESS_EQUAL : LESS);
      break;
    case '>':
      addToken(match('=') ? GREATER_EQUAL : GREATER);
      break;
    case '/':
      if (match('/')) {
        while (peek() != '\n' && !isAtEnd()) {
          advance();
        }
      } else {
        addToken(SLASH);
      }
      break;
    case ' ':
    case '\r':
      break;
    case '\n':
      ++line_;
      break;
    case '"':
      string();
      break;
    default:
      if (isDigit(c)) {
        number();
      } else if (isAlpha(c)) {
        identifier();
      } else {
       error(line_, "Unexpected character.");
      }
      break;
  }
}

char Scanner::advance() { return source_[current_++]; }

void Scanner::addToken(TokenType type) { addToken(type, nullptr); }

void Scanner::addToken(TokenType type, std::any literal) {
  std::string text{source_.substr(start_, current_ - start_)};
  tokens_.emplace_back(type, std::move(text), std::move(literal), line_);
}

bool Scanner::match(char expected) {
  if (isAtEnd()) {
    return false;
  }
  if (source_[current_] != expected) {
    return false;
  }
  ++current_;
  return true;
}

char Scanner::peek() {
  if (isAtEnd()) {
    return '\0';
  }
  return source_[current_];
}

char Scanner::peekNext() {
  if (current_ + 1 >= source_.size()) {
    return '\0';
  }
  return source_[current_ + 1];
}

void Scanner::string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') {
      ++line_;
    }
    advance();
  }
  if (isAtEnd()) {
   error(line_, "Unterminated string.");
    return;
  }
  advance();
  std::string value{source_.substr(start_ + 1, current_ - 2 - start_)};
  addToken(STRING, value);
}

bool Scanner::isDigit(char c) { return c >= '0' && c <= '9'; }

bool Scanner::isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

void Scanner::number() {
  while (isDigit(peek())) {
    advance();
  }
  if (peek() == '.' && isDigit(peekNext())) {
    advance();
  }
  while (isDigit(peek())) {
    advance();
  }
  addToken(NUMBER,
           std::stod(std::string{source_.substr(start_, current_ - start_)}));
}

void Scanner::identifier() {
  while (isAlphaNumeric(peek())) {
    advance();
  }
  std::string text = source_.substr(start_, current_ - start_);
  TokenType type;
  auto match = keywords.find(text);
  if (match == keywords.end()) {
    type = IDENTIFIER;
  } else {
    type = match->second;
  }
  addToken(type);
}