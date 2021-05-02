#include "AST.h"
#include "mid/walker/dumper/dumper.h"
#include "mid/walker/analyzer/sema.h"
#include "mid/walker/irbuilder/irbuilder.h"

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

void VariableDecl::Dump(Dumper *dumper) {
  dumper->visit(this);
}

void VariableDefAST::Dump(Dumper *dumper) {
  dumper->visit(this);
}

void BinaryStmt::Dump(Dumper *dumper) {
  dumper->visit(this);

}

void UnaryStmt::Dump(Dumper *dumper) {
  dumper->visit(this);

}

void ReturnStmt::Dump(mid::Dumper *dumper) {
  dumper->visit(this);
}

void CompoundStmt::Dump(Dumper *dumper) {
  dumper->visit(this);
}

void IfElseStmt::Dump(Dumper *dumper) {
  dumper->visit(this);
}

void CallStmt::Dump(Dumper *dumper) {
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

void WhileStmt::Dump(Dumper *dumper) {
  dumper->visit(this);
}

void TranslationUnitDecl::Dump(mid::Dumper *dumper) {
  dumper->visit(this);
}

TypeInfoPtr IntAST::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr CharAST::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr StringAST::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr VariableAST::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr VariableDecl::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr VariableDefAST::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr BinaryStmt::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr UnaryStmt::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr ReturnStmt::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr CompoundStmt::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr IfElseStmt::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr WhileStmt::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr CallStmt::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr ProtoTypeAST::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr FunctionDefAST::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr PrimTypeAST::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr FuncParamAST::SemAnalyze(SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

TypeInfoPtr TranslationUnitDecl::SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) {
  return analyzer->visit(this);
}

SSAPtr TranslationUnitDecl::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr IntAST::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr CharAST::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr StringAST::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr VariableAST::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr PrimTypeAST::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr VariableDecl::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr VariableDefAST::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr BinaryStmt::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr UnaryStmt::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr ReturnStmt::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr CompoundStmt::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr IfElseStmt::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr WhileStmt::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr CallStmt::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr BreakStmt::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr ContinueStmt::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr ProtoTypeAST::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr FunctionDefAST::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

SSAPtr FuncParamAST::CodeGeneAction(mid::IRBuilder *irbuilder) {
  return irbuilder->visit(this);
}

}
