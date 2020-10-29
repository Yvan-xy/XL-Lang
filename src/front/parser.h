#ifndef RJIT_FRONT_PARSER_H
#define RJIT_FRONT_PARSER_H

#include <map>
#include <iostream>

#include "front/lexer.h"
#include "define/AST.h"

using namespace RJIT::AST;

namespace RJIT::front {

    class Parser {
        Token curToken;
        Lexer *lexer;
        std::map<std::string, int> BinopPrecedence;

        void setPrecedence();

        bool isLeftParentheses();

        bool isRightParentheses();

        bool isComma();

        bool isEqualSign();

        bool isDefine();

        bool isExtern();

        bool isIf();

        bool isThen();

        bool isElse();

        bool isFor();

        bool isUnary();

        bool isEnd();

        bool isIn();

        bool isVar();

        bool isLeftBrace();

        bool isRightBrace();

        void nextToken() { curToken = lexer->nextToken(); }

        ASTPtr LogError(const std::string &info);

        bool isSemicolon();

        bool isColon();

    public:
        Parser() = default;

        explicit Parser(Lexer *lexer_) : lexer(lexer_) {
            setPrecedence();
            nextToken();    // Load first token
        }

        ASTPtr ParseParenExpr();

        ASTPtr ParseExpression();

        ASTPtr ParseVariableDecl();

        ASTPtr ParsePrimary();

        ASTPtr ParseIdentifier();

        ASTPtr ParseVariableDefine();

    };
}


#endif //RJIT_PARSER_H
