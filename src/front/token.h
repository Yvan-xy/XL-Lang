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

        TokenType getType() const { return type; }

        int getIntValue() const { return intValue; }

        uint8_t getCharValue() const { return charValue; }

        std::string getStringValue() const { return stringValue; }

        std::string getKeywordValue() const { return keywordValue; }

        std::string getOperatorValue() const { return operatorValue; }

        std::string getValue() const;

        int getLineNumber() const { return lineNumber; }

        int getPos() const { return pos; }

        void setToken(TokenType type, const std::string &value, int line, int position);

        void setPosition(int position) { pos = position; }

        void setLine(int line) { lineNumber = line; }

        void setType(TokenType type_) { type = type_; }

        std::string type2String();

        void dump();
    };
}

#endif