#ifndef RJIT_FRONT_LEX_H
#define RJIT_FRONT_LEX_H

#include "front/token.h"
#include <fstream>

namespace RJIT::front {
    class Lexer {
    private:
        const static std::string operators[];
        const static std::string keywords[];

        char ch = 0;
        std::ifstream inStream;
        int lineNumber = 1, pos = 0;

        bool isSpace() const { return std::isspace(ch); }

        void eatSpace() {
            while (isSpace()) {
                nextChar();
                if (inStream.eof()) break;
            }
        }

        void nextChar() {
            if (!inStream.eof()) {
                pos++;
                ch = inStream.get();
                if (ch == '\n' || ch == '\r') {
                    pos = 0;
                    lineNumber++;
                }
            }
        }

        bool isOperator() const;

        bool isEOF() {
            if (ch == -1 || inStream.eof()) {
                return true;
            }
            return false;
        }

        bool isKeyword(const std::string &value);

        bool matchChar(char ch_);

        void nextLine() {
            pos = 0;
            lineNumber++;
            inStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

    public:
        Lexer() = default;

        explicit Lexer(const std::string &filename) {
            inStream.open(filename, std::ios::in);
            if (!inStream.is_open()) {
                LogError(__LINE__, __FILE__, "File can't open.");
                exit(-1);
            }
            nextChar();
        }


        Token nextToken();

        static void LogInfo(int line, const char *filename, const std::string &info);

        static void LogError(int line, const char *filename, const std::string &info);
    };
}

#endif