#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>

#include "front/lexer.h"
#include "front/parser.h"

using namespace RJIT::front;

int main(int argc, char *argv[]) {
    Lexer lexer("a.c");
    Parser parser(&lexer);
    parser.Parse();
    parser.DumpAST();

}
