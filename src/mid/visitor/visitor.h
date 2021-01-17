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

        virtual void visit(VariableDeclAST *) {};

        virtual void visit(VariableDefAST *) {};

        virtual void visit(BinaryAST *) {};

        virtual void visit(UnaryAST *) {};

        virtual void visit(ReturnAST *) {};

        virtual void visit(BlockAST *) {};

        virtual void visit(IfElseAST *) {};

        virtual void visit(WhileAST *) {};

        virtual void visit(CallAST *) {};

        virtual void visit(FunctionDefAST *) {};

        virtual void visit(ProtoTypeAST *) {};

        virtual void visit(FuncParamAST *) {};

        virtual void visit(PrimTypeAST *) {};

    };
}

#endif //RJIT_VISITOR_H
