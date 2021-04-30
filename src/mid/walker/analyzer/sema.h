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

  class SemAnalyzer {
  private:
    bool in_func = false;
    TYPE::Type decl_type = TYPE::Type::Dam, ret_type = TYPE::Type::Void;
    EnvPtr symbol, alias;
    ASTPtr &rootNode;
  public:
    explicit SemAnalyzer(ASTPtr &ast) : rootNode(ast) {
//            symbol = MakeNestedMap<std::string, TypeInfoPtr>();
//            alias = MakeNestedMap<std::string, TypeInfoPtr>();
    }

    TypeInfoPtr SemAnalyze(IntAST *);

    TypeInfoPtr SemAnalyze(CharAST *);

    TypeInfoPtr SemAnalyze(StringAST *);

    TypeInfoPtr SemAnalyze(VariableAST *);

    TypeInfoPtr SemAnalyze(VariableDecl *);

    TypeInfoPtr SemAnalyze(VariableDefAST *);

    TypeInfoPtr SemAnalyze(BinaryStmt *);

    TypeInfoPtr SemAnalyze(UnaryStmt *);

    TypeInfoPtr SemAnalyze(ReturnStmt *);

    TypeInfoPtr SemAnalyze(BreakStmt *);

    TypeInfoPtr SemAnalyze(ContinueStmt *);

    TypeInfoPtr SemAnalyze(CompoundStmt *);

    TypeInfoPtr SemAnalyze(IfElseStmt *);

    TypeInfoPtr SemAnalyze(CallStmt *);

    TypeInfoPtr SemAnalyze(ProtoTypeAST *);

    TypeInfoPtr SemAnalyze(FunctionDefAST *);

    TypeInfoPtr SemAnalyze(FuncParamAST *);

    TypeInfoPtr SemAnalyze(PrimTypeAST *);

    TypeInfoPtr SemAnalyze(WhileStmt *);

    TypeInfoPtr SemAnalyze(TranslationUnitDecl *);

    void Analyze() { rootNode->SemAnalyze(this); }

    Guard NewEnv();

  };

  // print error message
  inline TypeInfoPtr LogError(const LoggerPtr &log, std::string &message, const std::string &id);

  inline TypeInfoPtr LogError(const LoggerPtr &log, std::string &message);

}

#endif //RJIT_SEMA_H
