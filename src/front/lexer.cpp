#include "front/lexer.h"
#include "define/color.h"

#include <iostream>

namespace RJIT::front {
  const std::string Lexer::operators[] = {
      "+", "-", "*", "/", "=", ">", "<",
      ">=", "<=", "==", "!=", "!", "|", "&",
      "(", ")", ",", ";", ":", "\"", "{",
      "}", "\'", ">>", "<<", "&&", "||",
      "+=", "-=", "*=", "/=", "%=", "&=",
      "|=", "^=", "<<=", ">>="
  };

  const std::string Lexer::keywords[] = {
      "def", "extern", "if", "else", "then", "for",
      "in", "binary", "unary", "var", "int32", "string",
      "char", "return", "uint32", "uint8", "int8", "uint",
      "void", "int", "while"
  };

  bool Lexer::isOperator() const {
    for (const auto &i : operators) {
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

  bool Lexer::matchChar(char ch_) {
    nextChar();
    if (ch == ch_) {
      return true;
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

    /* Eat comments. */
    while (ch == '#') {
      nextLine();
      nextChar();
      eatSpace();
      if (isEOF()) break;
    }
    if (inStream.eof()) {
      token.setToken(TokenType::END, "", lineNumber, pos);
    }

    if (std::isdigit(ch)) { // number token
      curPos = pos;
      curLine = lineNumber;
      do {
        value += ch;
        nextChar();
      } while (std::isdigit(ch));

      token.setToken(TokenType::Int, value, curLine, curPos);
    } else if (std::isalpha(ch) || ch == '_') { // keyword or identifier
      curPos = pos;
      curLine = lineNumber;
      do {
        value += ch;
        nextChar();
      } while (std::isalpha(ch) || std::isdigit(ch) || ch == '_');

      if (isKeyword(value)) {
        token.setToken(TokenType::Keyword, value, curLine, curPos);
      } else {
        token.setToken(TokenType::Identifier, value, curLine, curPos);
      }
    } else if (isOperator()) {  // operator token
      curPos = pos;
      curLine = lineNumber;
      bool eat = true;
      value += ch;

      bool isChar = false, isString = false;

      /* Check if >=, <=, ==, != */
      if (ch == '=' || ch == '>' || ch == '<' || ch == '!'
          || ch == '+' || ch == '-' || ch == '*' || ch == '/') {
        eat = false;
        nextChar();
        if (ch == '=') {
          value += ch;
          eat = true;
        }
      } else if (ch == '\'') {    // char token
        eat = true;
        isChar = true;
        if (matchChar('\'')) {
          LogError(__LINE__, __FILE__, "Expect a char here.");
        }
        value = std::string("") + ch;
        if (!matchChar('\'')) {
          LogError(__LINE__, __FILE__, "Expect a ' here.");
        }
      } else if (ch == '"') { // string token
        eat = true;
        isString = true;
        value = "";
        if (matchChar('"')) {
          LogError(__LINE__, __FILE__, "Expect a string here.");
        }
        do {
          value += ch;
          nextChar();
          if (isEOF()) {
            LogError(__LINE__, __FILE__, "Expect a \" here");
          }
        } while (ch != '"');
      }

      if (eat) {
        nextChar(); // eat space
      }
      if (isChar) {
        token.setToken(TokenType::Char, value, curLine, curPos);
      } else if (isString) {
        token.setToken(TokenType::String, value, curLine, curPos);
      } else {
        token.setToken(TokenType::Operator, value, curLine, curPos);
      }
    }

//        token.dump();

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