#include "front/lexer.h"
#include "define/color.h"

#include <iostream>

namespace RJIT::front {
    const std::string Lexer::operators[] = {
            "+", "-", "*", "/", "=", ">", "<",
            ">=", "<=", "==", "!=", "!", "|", "&",
            "(", ")", ",", ";", ":", "\"", "{",
            "}"
    };

    const std::string Lexer::keywords[] = {
            "def", "extern", "if", "else", "then", "for",
            "in", "binary", "unary", "var", "int", "string",
            "char", "return"
    };

    bool Lexer::isOperator() const {
        for (auto i : operators) {
            if (ch == i[0]) {
                return true;
            }
        }
        return false;
    }

    bool Lexer::isKeyword(const std::string &value) {
        for (const auto &i : keywords) {
            if (value == i) {
                return true;
            }
        }
        return false;
    }

    Token Lexer::nextToken() {
        int curPos, curLine;
        Token token;
        std::string value;

        while (isSpace()) { eatSpace(); }

        if (isEOF()) {
            token.setType(TokenType::END);
        }

        if (std::isdigit(ch)) {
            curPos = pos;
            curLine = lineNumber;
            do {
                value += ch;
                nextChar();
            } while (std::isdigit(ch));

            token.setToken(TokenType::Int, value, curLine, curPos);
        } else if (std::isalpha(ch) || ch == '_') {
            curPos = pos;
            curLine = lineNumber;
            do {
                value += ch;
                nextChar();
            } while (std::isalpha(ch) || std::isdigit(ch) || ch == '_');

//            nextChar(); // eat end
            if (isKeyword(value)) {
                token.setToken(TokenType::Keyword, value, curLine, curPos);
            } else {
                token.setToken(TokenType::Identifier, value, curLine, curPos);
            }
        } else if (isOperator()) {
            curPos = pos;
            curLine = lineNumber;
            bool eat = true;
            value += ch;
            if (ch == '=' || ch == '>' || ch == '<' || ch == '!') {
                eat = false;
                nextChar();
                if (ch == '=') {
                    value += ch;
                    eat = true;
                }
            }
            if (eat) {
                nextChar(); // eat space
            }

            token.setToken(TokenType::Operator, value, curLine, curPos);
        }


        token.dump();
        return token;
    }

    void Lexer::LogInfo(int line, const char *filename, const std::string &info) {
        Color::Modifier green(Color::Code::FG_GREEN);
        Color::Modifier def(Color::Code::FG_DEFAULT);
        std::cout << ": " << filename << ":"
                  << line << ": " << green << "info: "
                  << def << info << std::endl;

    }

    void Lexer::LogError(int line, const char *filename, const std::string &info) {
        Color::Modifier red(Color::Code::FG_RED);
        Color::Modifier def(Color::Code::FG_DEFAULT);
        std::cout << ": " << filename << ":"
                  << line << ": " << red << "error: "
                  << def << info << std::endl;
        exit(-1);
    }
}   // RJIT::front