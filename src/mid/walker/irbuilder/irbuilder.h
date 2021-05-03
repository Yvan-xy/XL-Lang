#ifndef RJIT_IRBUILDER_H
#define RJIT_IRBUILDER_H

#include "mid/ir/value.h"
#include "mid/ir/module.h"
#include "mid/visitor/visitor.h"

namespace RJIT::mid {

class IRBuilder : Visitor<SSAPtr> {
private:
  bool       _in_func;
  Module     _module;
  ASTPtr    &_translation_decl_unit;

public:
  explicit IRBuilder(ASTPtr &ast)
    : _translation_decl_unit(ast) { _in_func = false; }

  // get module
  Module &module() { return _module; }

  // new value environment
  Guard NewEnv() { return _module.NewEnv(); }

  SSAPtr visit(IntAST              *) override;
  SSAPtr visit(CharAST             *) override;
  SSAPtr visit(StringAST           *) override;
  SSAPtr visit(VariableAST         *) override;
  SSAPtr visit(VariableDecl        *) override;
  SSAPtr visit(VariableDefAST      *) override;
  SSAPtr visit(BinaryStmt          *) override;
  SSAPtr visit(UnaryStmt           *) override;
  SSAPtr visit(ReturnStmt          *) override;
  SSAPtr visit(BreakStmt           *) override;
  SSAPtr visit(ContinueStmt        *) override;
  SSAPtr visit(CompoundStmt        *) override;
  SSAPtr visit(IfElseStmt          *) override;
  SSAPtr visit(CallStmt            *) override;
  SSAPtr visit(ProtoTypeAST        *) override;
  SSAPtr visit(FunctionDefAST      *) override;
  SSAPtr visit(FuncParamAST        *) override;
  SSAPtr visit(PrimTypeAST         *) override;
  SSAPtr visit(WhileStmt           *) override;
  SSAPtr visit(TranslationUnitDecl *) override;

  void EmitIR() { _translation_decl_unit->CodeGeneAction(this); }

  // print error message
  SSAPtr LogError(const front::LoggerPtr &log, std::string &message);
  SSAPtr LogError(const front::LoggerPtr &log, std::string &message, const std::string &id);
};

}

#endif //RJIT_IRBUILDER_H
