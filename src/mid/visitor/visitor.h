#ifndef RJIT_MID_VISITOR_H
#define RJIT_MID_VISITOR_H

#include "define/AST.h"


namespace RJIT::mid {
using namespace RJIT::AST;

template<typename RETURN_TYPE>
class Visitor {
public:
  Visitor() = default;

  virtual RETURN_TYPE visit(IntAST              *) = 0;
  virtual RETURN_TYPE visit(CharAST             *) = 0;
  virtual RETURN_TYPE visit(StringAST           *) = 0;
  virtual RETURN_TYPE visit(VariableAST         *) = 0;
  virtual RETURN_TYPE visit(VariableDecl        *) = 0;
  virtual RETURN_TYPE visit(VariableDefAST      *) = 0;
  virtual RETURN_TYPE visit(BinaryStmt          *) = 0;
  virtual RETURN_TYPE visit(UnaryStmt           *) = 0;
  virtual RETURN_TYPE visit(ReturnStmt          *) = 0;
  virtual RETURN_TYPE visit(BreakStmt           *) = 0;
  virtual RETURN_TYPE visit(ContinueStmt        *) = 0;
  virtual RETURN_TYPE visit(CompoundStmt        *) = 0;
  virtual RETURN_TYPE visit(IfElseStmt          *) = 0;
  virtual RETURN_TYPE visit(WhileStmt           *) = 0;
  virtual RETURN_TYPE visit(CallStmt            *) = 0;
  virtual RETURN_TYPE visit(FunctionDefAST      *) = 0;
  virtual RETURN_TYPE visit(ProtoTypeAST        *) = 0;
  virtual RETURN_TYPE visit(FuncParamAST        *) = 0;
  virtual RETURN_TYPE visit(PrimTypeAST         *) = 0;
  virtual RETURN_TYPE visit(TranslationUnitDecl *) = 0;
};
}

#endif //RJIT_VISITOR_H
