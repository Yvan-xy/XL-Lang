#ifndef RJIT_FRONT_PARSER_H
#define RJIT_FRONT_PARSER_H

#include "front/lexer.h"
#include "define/AST.h"
namespace RJIT::front {

    class Parser {
        const Lexer *const lexer;
    public:
        Parser() = default;

        explicit Parser(const Lexer *const lexer_) : lexer(lexer_) {}
    };
}


#endif //RJIT_PARSER_H
