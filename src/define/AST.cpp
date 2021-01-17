#include "AST.h"
#include "mid/walker/dumper/dumper.h"
#include "mid/walker/analyzer/sema.h"

#include <iostream>

using namespace std;
using namespace RJIT::TYPE;
using namespace RJIT::mid;
using namespace RJIT::mid::analyzer;

namespace RJIT::AST {
    PrimASTPtr MakePrimeAST(front::LoggerPtr logger, TYPE::Type type) {
        PrimASTPtr ast = std::make_unique<PrimTypeAST>(type);
        ast->setLogger(std::move(logger));
        return ast;
    }

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

    TypeInfoPtr IntAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr CharAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr StringAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr VariableAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr VariableDeclAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr VariableDefAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr BinaryAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr UnaryAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr ReturnAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr BlockAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr IfElseAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr WhileAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr CallAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr ProtoTypeAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr FunctionDefAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr PrimTypeAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

    TypeInfoPtr FuncParamAST::SemAnalyze(SemAnalyzer *analyzer) {
        return analyzer->SemAnalyze(this);
    }

}
