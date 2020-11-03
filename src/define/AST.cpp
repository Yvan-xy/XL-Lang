#include "AST.h"
#include "mid/walker/dumper.h"

#include <iostream>

using namespace std;
using namespace RJIT::mid;

namespace RJIT::AST {
    void IntAST::Dump(Dumper *dumper) {
        dumper->visit(this);
    }

    void CharAST::Dump(Dumper *dumper) {
        dumper->visit(this);
    }

    void StringAST::Dump(Dumper *dumper) {
        dumper->visit(this);
    }

    void VariableAST::Dump(Dumper *dumper) {
        dumper->visit(this);
    }

    void VariableDeclAST::Dump(Dumper *dumper) {
        dumper->visit(this);
    }

    void VariableDefAST::Dump(Dumper *dumper) {
        dumper->visit(this);
    }

    void BinaryAST::Dump(Dumper *dumper) {
        dumper->visit(this);

    }

    void UnaryAST::Dump(Dumper *dumper) {
        dumper->visit(this);

    }

    void BlockAST::Dump(Dumper *dumper) {
        dumper->visit(this);
    }

    void IfElseAST::Dump(Dumper *dumper) {
        dumper->visit(this);

    }

    void CallAST::Dump(Dumper *dumper) {
        dumper->visit(this);

    }

    void ProtoTypeAST::Dump(Dumper *dumper) {
        dumper->visit(this);

    }

    void FunctionDefAST::Dump(Dumper *dumper) {
        dumper->visit(this);

    }

    void FuncParamAST::Dump(Dumper *dumper) {
        dumper->visit(this);

    }

    void PrimTypeAST::Dump(Dumper *dumper) {
        dumper->visit(this);

    }

    void WhileAST::Dump(Dumper *dumper) {
        dumper->visit(this);

    }


}
