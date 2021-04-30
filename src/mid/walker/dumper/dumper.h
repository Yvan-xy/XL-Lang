#ifndef RJIT_MID_DUMPER_H
#define RJIT_MID_DUMPER_H

#include <ostream>
#include <fstream>
#include "mid/visitor/visitor.h"

namespace RJIT::mid {
    class Dumper : public Visitor {
    private:
        int depth = 0;
        std::ostream &os;
    public:
        explicit Dumper(std::ostream &os_) : os(os_) {}

        std::string getPadding() const {
            std::string pad;
            for (int i = 0; i < depth; i++) {
                pad += "\t";
            }
            return pad;
        }

        void incDepth() { depth++; }

        void decDepth() { depth--; }

        void visit(IntAST *) override;

        void visit(CharAST *) override;

        void visit(StringAST *) override;

        void visit(VariableAST *) override;

        void visit(VariableDecl *) override;

        void visit(VariableDefAST *) override;

        void visit(BinaryStmt *) override;

        void visit(UnaryStmt *) override;

        void visit(ReturnStmt *) override;

        void visit(BreakStmt *) override;

        void visit(ContinueStmt *) override;

        void visit(CompoundStmt *) override;

        void visit(IfElseStmt *) override;

        void visit(CallStmt *) override;

        void visit(ProtoTypeAST *) override;

        void visit(FunctionDefAST *) override;

        void visit(FuncParamAST *) override;

        void visit(PrimTypeAST *) override;

        void visit(WhileStmt *) override;

        void visit(TranslationUnitDecl *) override;
    };
}

#endif //RJIT_DUMPER_H
