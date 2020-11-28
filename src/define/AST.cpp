#include "AST.h"
#include "mid/walker/dumper/dumper.h"
#include "mid/walker/analyzer/symbol.h"

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

    void ReturnAST::Dump(mid::Dumper *dumper) {
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

    void IntAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void CharAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void StringAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void VariableAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void VariableDeclAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void VariableDefAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void BinaryAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void UnaryAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void ReturnAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void BlockAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void IfElseAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void WhileAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void CallAST::Symbol(mid::analyzer::Symbol *symbol_) {
        symbol_->visit(this);
    }

    void ProtoTypeAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void FunctionDefAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void PrimTypeAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

    void FuncParamAST::Symbol(mid::analyzer::Symbol *symbol) {
        symbol->visit(this);
    }

}
