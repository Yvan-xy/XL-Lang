#ifndef RJIT_SEMA_H
#define RJIT_SEMA_H

#include "define/type.h"
#include "front/logger.h"
#include "lib/guard.h"
#include "lib/nestedmap.h"
#include "mid/visitor/visitor.h"

using namespace RJIT::lib;
using namespace RJIT::TYPE;
using namespace RJIT::front;

namespace RJIT::mid::analyzer {

using EnvPtr = lib::Nested::NestedMapPtr<std::string, TypeInfoPtr>;

class SemAnalyzer : Visitor<TypeInfoPtr> {
private:
  EnvPtr     _alias;
  EnvPtr     _symbol;
  ASTPtr    &_rootNode;
  bool       _in_func = false;
  TYPE::Type _decl_type = TYPE::Type::Dam;
  TYPE::Type _ret_type = TYPE::Type::Void;

public:
  explicit SemAnalyzer(ASTPtr &ast) : _rootNode(ast) {}

  TypeInfoPtr visit(IntAST              *) override;
  TypeInfoPtr visit(CharAST             *) override;
  TypeInfoPtr visit(StringAST           *) override;
  TypeInfoPtr visit(VariableAST         *) override;
  TypeInfoPtr visit(VariableDecl        *) override;
  TypeInfoPtr visit(VariableDefAST      *) override;
  TypeInfoPtr visit(BinaryStmt          *) override;
  TypeInfoPtr visit(UnaryStmt           *) override;
  TypeInfoPtr visit(ReturnStmt          *) override;
  TypeInfoPtr visit(BreakStmt           *) override;
  TypeInfoPtr visit(ContinueStmt        *) override;
  TypeInfoPtr visit(CompoundStmt        *) override;
  TypeInfoPtr visit(IfElseStmt          *) override;
  TypeInfoPtr visit(CallStmt            *) override;
  TypeInfoPtr visit(ProtoTypeAST        *) override;
  TypeInfoPtr visit(FunctionDefAST      *) override;
  TypeInfoPtr visit(FuncParamAST        *) override;
  TypeInfoPtr visit(PrimTypeAST         *) override;
  TypeInfoPtr visit(WhileStmt           *) override;
  TypeInfoPtr visit(TranslationUnitDecl *) override;

  void Analyze() { _rootNode->SemAnalyze(this); }

  Guard NewEnv();

};

// print error message
inline TypeInfoPtr LogError(const LoggerPtr &log, std::string &message, const std::string &id);

inline TypeInfoPtr LogError(const LoggerPtr &log, std::string &message);

}

#endif //RJIT_SEMA_H
