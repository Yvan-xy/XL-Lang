#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>

#include "front/lexer.h"

using namespace RJIT::front;

int main(int argc, char *argv[]) {
    Lexer lexer("a.c");
    Token token;
    do {
        token = lexer.nextToken();
    } while(token.getTokenType() != TokenType::END);
}