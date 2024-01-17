#pragma once

#include <any>
#include <map>
#include <string>

enum TokenType {
  // Single-character tokens.
  LEFT_PAREN,   // `(`
  RIGHT_PAREN,  // `)`
  LEFT_BRACE,   // `{`
  RIGHT_BRACE,  // `}`
  COMMA,        // `,`
  DOT,          // `.`
  MINUS,        // `-`
  PLUS,         // `+`
  SEMICOLON,    // `;`
  SLASH,        // `/`
  STAR,         // `*`
  // One or two character tokens.
  BANG,           // `!`
  BANG_EQUAL,     // `!=`
  EQUAL,          // `=`
  EQUAL_EQUAL,    // `==`
  GREATER,        // `>`
  GREATER_EQUAL,  // `>=`
  LESS,           // `<`
  LESS_EQUAL,     // `<=`
  // Literals.
  IDENTIFIER,  // 标识符
  STRING,      // 字符串
  NUMBER,      // 数值
  // Keywords.
  AND,     // `and`
  CLASS,   // `class`
  ELSE,    // `else`
  FALSE,   // `false`
  FUN,     // `fun`
  FOR,     // `for`
  IF,      // `if`
  NIL,     // `nil`
  OR,      // `or`
  PRINT,   // `print`
  RETURN,  // `return`
  SUPER,   // `super`
  THIS,    // `this`
  TRUE,    // `true`
  VAR,     // `var`
  WHILE,   // `while`

  END_OF_FILE  // 文件结束符
};

const std::map<TokenType, std::string> strings{
    {LEFT_PAREN, "LEFT_PAREN"},
    {RIGHT_PAREN, "RIGHT_PAREN"},
    {LEFT_BRACE, "LEFT_BRACE"},
    {RIGHT_BRACE, "RIGHT_BRACE"},
    {COMMA, "COMMA"},
    {DOT, "DOT"},
    {MINUS, "MINUS"},
    {PLUS, "PLUS"},
    {SEMICOLON, "SEMICOLON"},
    {SLASH, "SLASH"},
    {STAR, "STAR"},
    {BANG, "BANG"},
    {BANG_EQUAL, "BANG_EQUAL"},
    {EQUAL, "EQUAL"},
    {EQUAL_EQUAL, "EQUAL_EQUAL"},
    {GREATER, "GREATER"},
    {GREATER_EQUAL, "GREATER_EQUAL"},
    {LESS, "LESS"},
    {LESS_EQUAL, "LESS_EQUAL"},
    {IDENTIFIER, "IDENTIFIER"},
    {STRING, "STRING"},
    {NUMBER, "NUMBER"},
    {AND, "AND"},
    {CLASS, "CLASS"},
    {ELSE, "ELSE"},
    {FALSE, "FALSE"},
    {FUN, "FUN"},
    {FOR, "FOR"},
    {IF, "IF"},
    {NIL, "NIL"},
    {OR, "OR"},
    {PRINT, "PRINT"},
    {RETURN, "RETURN"},
    {SUPER, "SUPER"},
    {THIS, "THIS"},
    {TRUE, "TRUE"},
    {VAR, "VAR"},
    {WHILE, "WHILE"},
    {END_OF_FILE, "END_OF_FILE"},
};

const std::map<std::string, TokenType> keywords{
    {"and", AND},   {"class", CLASS}, {"else", ELSE},     {"false", FALSE},
    {"for", FOR},   {"fun", FUN},     {"if", IF},         {"nil", NIL},
    {"or", OR},     {"print", PRINT}, {"return", RETURN}, {"super", SUPER},
    {"this", THIS}, {"true", TRUE},   {"var", VAR},       {"while", WHILE},
};

class Token {
 public:
  Token(TokenType type, std::string lexeme, std::any literal, int line)
      : type_(type),
        lexeme_(std::move(lexeme)),
        literal_(std::move(literal)),
        line_(line) {}

  TokenType getType() { return type_; }

  std::string toString() const;

  TokenType type_;
  std::string lexeme_;
  std::any literal_;
  int line_;
};