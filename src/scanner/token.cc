#include "token.h"

#include <any>
#include <string>

std::string Token::toString() const {
  std::string literal_text;
  switch (type_) {
    case (IDENTIFIER):
      literal_text = lexeme_;
      break;
    case (STRING):
      literal_text = std::any_cast<std::string>(literal_);
    case (NUMBER):
      literal_text = std::to_string(std::any_cast<double>(literal_));
      break;
    case (TRUE):
      literal_text = "true";
      break;
    case (FALSE):
      literal_text = "false";
      break;
    default:
      literal_text = "nil";
  }
  return strings.at(type_) + " " + lexeme_ + " " + literal_text;
}