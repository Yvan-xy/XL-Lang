#include "dumper.h"
#include <iostream>

using namespace std;

namespace RJIT::mid {

  void Dumper::visit(IntAST *node) {
    os << "[ \"" << node->getValue() << "\" : " << "int ]";
  }

  void Dumper::visit(CharAST *node) {
    os << "[ \"" << node->getValue() << "\" : " << "char ]";

  }

  void Dumper::visit(StringAST *node) {
    os << "[ \"" << node->getValue() << "\" : " << "char ]";

  }

  void Dumper::visit(VariableAST *node) {
    os << "[ \"" << node->getName() << "\" : " << node->getTypeStr() <<" ]";
  }

  void Dumper::visit(VariableDecl *node) {
    os << "[ ";
    for (const auto &i : node->getDefs()) {
      i->Dump(this);
      os << " ";
    }
    os << ": VariableDeclare ]";
  }

  void Dumper::visit(VariableDefAST *node) {
    os << "[ \"" << node->getIdentifier() << "\"";
    if (node->hasInit()) {
      os << " : ";
      node->getInitValue()->Dump(this);
    }
    os << " ]";
  }

  void Dumper::visit(BinaryStmt *node) {
    os << "[ ";
    node->getLHS()->Dump(this);
    os << " : ";
    node->getRHS()->Dump(this);
    os << " : " << node->getOpStr() << " ]";
  }

  void Dumper::visit(UnaryStmt *node) {
    os << "[ ";
    node->Operand()->Dump(this);
    os << " : " << node->getOpString() << " ]";
  }

  void Dumper::visit(ReturnStmt *node) {
    os << "[ return";
    if (node->hasReturnVal()) {
      os << " ";
      node->getReturn()->Dump(this);
    }
    os << " ]";
  }

  void Dumper::visit(BreakStmt *node) {
    os << "[ break ]";
  }

  void Dumper::visit(ContinueStmt *node) {
    os << "[ continue ]";
  }

  void Dumper::visit(CompoundStmt *node) {
    os << "[\n";
    incDepth();
    auto end = --(node->stmts().end());
    for (const auto &i : node->stmts()) {
      os << getPadding();
      i->Dump(this);
      if (i != *end) os << "\n";
    }
    decDepth();
    os << "\n" << getPadding() << "]";
  }

  void Dumper::visit(IfElseStmt *node) {
    os << "if ( ";
    node->getCondition()->Dump(this);
    os << " ) ";
    node->getThen()->Dump(this);
    if (node->getElse() != nullptr) {
      os << getPadding() << "else ";
      node->getElse()->Dump(this);
    }
    os << "\n";
  }

  void Dumper::visit(CallStmt *node) {
    os << "[ \"" << node->getSymbol() << "\" : function ] ";
    os << "(";

    auto tail = --(node->getArgs().end());
    for (const auto &i :node->getArgs()) {
      i->Dump(this);
      if (i != *tail) os << ", ";
    }
    os << ")";
  }

  void Dumper::visit(ProtoTypeAST *node) {
    os << "\"" << node->getFuncName() << "\" : function (";
    auto tail = --(node->getArgs().end());
    for (const auto &i : node->getArgs()) {
      i->Dump(this);
      if (i != *tail) os << ", ";
    }
    os << ") " << node->getReturnTypeStr() << " ";
  }

  void Dumper::visit(FunctionDefAST *node) {
    node->getProtoType()->Dump(this);
    node->getBody()->Dump(this);
  }

  void Dumper::visit(FuncParamAST *node) {
    os << "\"" << node->getIdentifier() << "\" : "
       << node->getTypeStr();
  }

  void Dumper::visit(PrimTypeAST *node) {}

  void Dumper::visit(WhileStmt *node) {
    os << "while ( ";
    node->getCondition()->Dump(this);
    os << " ) ";
    node->getBlock()->Dump(this);
    os << "\n";
  }

  void Dumper::visit(TranslationUnitDecl *node) {
    for (const auto &it : node->Decls()) {
      it->Dump(this);
      os << "\n" << endl;
    }
  }
}