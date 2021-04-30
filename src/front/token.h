#ifndef RJIT_FRONT_TOKEN_H
#define RJIT_FRONT_TOKEN_H

#include <string>

namespace RJIT::front {
  enum class TokenType {
    Keyword, Operator, Identifier, Int, Char, String, END
  };

  class Token {
  private:
    TokenType type;
    int intValue;
    uint8_t charValue;
    std::string stringValue;
    std::string keywordValue;
    std::string operatorValue;
    std::string identifierValue;
    int lineNumber = -1, pos = -1;
  public:
    Token() = default;

    TokenType getTokenType() const { return type; }

    int getIntValue() const { return intValue; }

    uint8_t getCharValue() const { return charValue; }

    const std::string &getStringValue() const { return stringValue; }

    const std::string &getKeywordValue() const { return keywordValue; }

    const std::string &getOperValue() const { return operatorValue; }

    const std::string &getIdentifierValue() const { return identifierValue; }

    std::string getValue() const;

    int getLineNumber() const { return lineNumber; }

    int getPosition() const { return pos; }

    void setToken(TokenType type, const std::string &value, int line, int position);

    void setPosition(int position) { pos = position; }

    void setLine(int line) { lineNumber = line; }

    void setType(TokenType type_) { type = type_; }

    std::string type2String();

    bool isIdentifier() const { return type == TokenType::Identifier; }

    bool isFunctionName() const { return type == TokenType::Identifier; }

    bool isOper() const { return type == TokenType::Operator; }

    bool isKeyword() const { return type == TokenType::Keyword; }

    bool isInt() const { return type == TokenType::Int; }

    bool isChar() const { return type == TokenType::Char; }

    bool isString() const { return type == TokenType::String; }

    void dump();
  };
}

#endif