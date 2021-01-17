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
        os << "[ \"" << node->getName() << "\" : " << "Variable ]";
    }

    void Dumper::visit(VariableDeclAST *node) {
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

    void Dumper::visit(BinaryAST *node) {
        os << "[ ";
        node->getLHS()->Dump(this);
        os << " : ";
        node->getRHS()->Dump(this);
        os << " : " << node->getOpStr() << " ]";
    }

    void Dumper::visit(UnaryAST *node) {
        os << "[ ";
        node->getOperand()->Dump(this);
        os << " : " << node->getOper() << " ]";
    }

    void Dumper::visit(ReturnAST *node) {
        os << "[ ";
        node->getReturn()->Dump(this);
        os << " ]";
    }

    void Dumper::visit(BlockAST *node) {
        os << "[\n";
        incDepth();
        auto end = --(node->getStmts().end());
        for (const auto &i : node->getStmts()) {
            os << getPadding();
            i->Dump(this);
            if (i != *end) os << "\n";
        }
        decDepth();
        os << "\n" << getPadding() << "]";
    }

    void Dumper::visit(IfElseAST *node) {
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

    void Dumper::visit(CallAST *node) {
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

    void Dumper::visit(WhileAST *node) {
        os << "while ( ";
        node->getCondition()->Dump(this);
        os << " ) ";
        node->getBlock()->Dump(this);
        os << "\n";
    }
}