#ifndef RJIT_MID_VISITOR_H
#define RJIT_MID_VISITOR_H

#include "define/AST.h"


namespace RJIT::mid {
    using namespace RJIT::AST;

    class Visitor {
    public:
        Visitor() = default;

        virtual void visit(IntAST *) = 0;

        virtual void visit(CharAST *) = 0;

        virtual void visit(StringAST *) = 0;

        virtual void visit(VariableAST *) = 0;

        virtual void visit(VariableDeclAST *) = 0;

        virtual void visit(VariableDefAST *) = 0;

        virtual void visit(BinaryAST *) = 0;

        virtual void visit(UnaryAST *) = 0;

        virtual void visit(ReturnAST *) = 0;

        virtual void visit(BlockAST *) = 0;

        virtual void visit(IfElseAST *) = 0;

        virtual void visit(WhileAST *) = 0;

        virtual void visit(CallAST *) = 0;

        virtual void visit(FunctionDefAST *) = 0;

        virtual void visit(ProtoTypeAST *) = 0;

        virtual void visit(FuncParamAST *) = 0;

        virtual void visit(PrimTypeAST *) = 0;

    };
}

#endif //RJIT_VISITOR_H
