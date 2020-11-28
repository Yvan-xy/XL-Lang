#ifndef RJIT_SYMBOL_H
#define RJIT_SYMBOL_H

#include "define/type.h"
#include "mid/visitor/visitor.h"
#include "lib/nestedmap/nestedmap.h"

#include <memory>
using namespace RJIT::lib;
using namespace RJIT::TYPE;

namespace RJIT::mid::analyzer {

    using EnvPtr = lib::Nested::NestedMapPtr<std::string, TypeInfoPtr>;

    class Symbol : public Visitor {
    private:
        EnvPtr env_;
        ASTPtr &rootNode;


    public:
        explicit Symbol(ASTPtr &ast) : rootNode(ast) {
            env_ = MakeNestedMap<std::string, TypeInfoPtr>();
//            env_ = std::make_shared<NestedMap<std::string, TypeInfoPtr>>(env_);
        }

        void visit(IntAST *) override;

        void visit(CharAST *) override;

        void visit(StringAST *) override;

        void visit(VariableAST *) override;

        void visit(VariableDeclAST *) override;

        void visit(VariableDefAST *) override;

        void visit(BinaryAST *) override;

        void visit(UnaryAST *) override;

        void visit(ReturnAST *) override;

        void visit(BlockAST *) override;

        void visit(IfElseAST *) override;

        void visit(CallAST *) override;

        void visit(ProtoTypeAST *) override;

        void visit(FunctionDefAST *) override;

        void visit(FuncParamAST *) override;

        void visit(PrimTypeAST *) override;

        void visit(WhileAST *) override;
    };

}


#endif //RJIT_SYMBOL_H
