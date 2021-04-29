#include "front/token.h"
#include "define/color.h"

#include <iostream>

namespace RJIT::front {

  void Token::setToken(TokenType type_, const std::string &value, int line, int position) {
    this->setType(type_);
    this->setLine(line);
    this->setPosition(position);
    switch (type) {
      case TokenType::Keyword: {
        this->keywordValue = value;
        break;
      }
      case TokenType::Operator: {
        this->operatorValue = value;
        break;
      }
      case TokenType::Identifier: {
        this->identifierValue = value;
        break;
      }
      case TokenType::Int: {
        this->intValue = std::stoi(value);
        break;
      }
      case TokenType::Char: {
        this->charValue = value.c_str()[0];
        break;
      }
      case TokenType::String: {
        this->stringValue = value;
        break;
      }
      case TokenType::END: {
        break;
      }
    }
  }

  std::string Token::getValue() const {
    std::string value;
    switch (type) {
      case TokenType::Keyword:
        return keywordValue;
      case TokenType::Operator:
        return operatorValue;
      case TokenType::Identifier:
        return identifierValue;
      case TokenType::Int:
        return std::to_string(intValue);
      case TokenType::Char:
        return std::string(1, charValue);
      case TokenType::String:
        return stringValue;
      case TokenType::END:
        return "";
    }
    return "";
  }

  std::string Token::type2String() {
    std::string typeStr;
    switch (type) {
      case TokenType::Keyword:
        typeStr = "Keyword";
        break;
      case TokenType::Operator:
        typeStr = "Operator";
        break;
      case TokenType::Identifier:
        typeStr = "Identifier";
        break;
      case TokenType::Int:
        typeStr = "Int";
        break;
      case TokenType::Char:
        typeStr = "Char";
        break;
      case TokenType::String:
        typeStr = "String";
        break;
      case TokenType::END:
        typeStr = "END";
        break;
    }
    return typeStr;
  }

  void Token::dump() {
    Color::Modifier green(Color::Code::FG_GREEN);
    Color::Modifier def(Color::Code::FG_DEFAULT);
    std::cout << green << "Line: " << def << lineNumber << "\t"
              << green << "Pos: " << def << pos << "\t"
              << green << "Type: " << def << type2String() << "\t"
              << green << "Value: " << def << getValue() << std::endl;

  }

} // RJIT::front
