#ifndef RJIT_FRONT_PARSER_H
#define RJIT_FRONT_PARSER_H

#include <map>
#include <iostream>
#include <optional>

#include "front/lexer.h"
#include "define/AST.h"
#include "mid/walker/dumper.h"

using namespace RJIT::AST;

namespace RJIT::front {

    class Parser {
        Token curToken;
        Lexer *lexer;
        std::map<std::string, int> BinopPrecedence;

        ASTPtr rootNode;

        void setPrecedence();

        int getPrecedence();

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

        bool isWhile();

        bool isUnary();

        bool isEnd();

        bool isIn();

        bool isVar();

        bool isLeftBrace();

        bool isRightBrace();

        bool isSemicolon();

        bool isColon();

        bool isConst();

        TYPE::Type getType();

        void nextToken() { curToken = lexer->nextToken(); }

        ASTPtr LogError(const std::string &info);

        ASTPtr ParseParenExpr();

        ASTPtr ParseExpression();

        ASTPtr ParseBare();

        ASTPtr ParseUnary();

        ASTPtr ParseBinaryOPRHS(int prec, ASTPtr lhs);

        ASTPtr ParseVariableDecl();

        ASTPtr ParsePrimary();

        ASTPtr ParseIdentifier();

        ASTPtr ParseVariableDefine();

        ASTPtr ParseIfElse();

        ASTPtr ParseWhile();

        ASTPtr ParseConst();

        ASTPtr ParseFunctionDef();

        ASTPtr ParseBlock();

        ASTPtr ParseTop();

    public:
        Parser() = default;

        explicit Parser(Lexer *lexer_) : lexer(lexer_) {
            setPrecedence();
            nextToken();    // Load first token
        }


        void Parse();

        void DumpAST() {
            RJIT::mid::Dumper dumper(std::cout);
            rootNode->Dump(reinterpret_cast<mid::Dumper *>(&dumper));
        }

    };
}


#endif //RJIT_PARSER_H
