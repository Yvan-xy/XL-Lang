#ifndef RJIT_MID_VISITOR_H
#define RJIT_MID_VISITOR_H

#include "define/AST.h"


namespace RJIT::mid {
  using namespace RJIT::AST;

  class Visitor {
  public:
    Visitor() = default;

    virtual void visit(IntAST *) {};

    virtual void visit(CharAST *) {};

    virtual void visit(StringAST *) {};

    virtual void visit(VariableAST *) {};

    virtual void visit(VariableDecl *) {};

    virtual void visit(VariableDefAST *) {};

    virtual void visit(BinaryStmt *) {};

    virtual void visit(UnaryStmt *) {};

    virtual void visit(ReturnStmt *) {};

    virtual void visit(BreakStmt *) {};

    virtual void visit(ContinueStmt *) {};

    virtual void visit(CompoundStmt *) {};

    virtual void visit(IfElseStmt *) {};

    virtual void visit(WhileStmt *) {};

    virtual void visit(CallStmt *) {};

    virtual void visit(FunctionDefAST *) {};

    virtual void visit(ProtoTypeAST *) {};

    virtual void visit(FuncParamAST *) {};

    virtual void visit(PrimTypeAST *) {};

  };
}

#endif //RJIT_VISITOR_H
