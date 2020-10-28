#ifndef RJIT_AST_H
#define RJIT_AST_H

namespace RJIT::AST {
    class ExprAST;

    typedef std::shared_ptr<ExprAST> ASTPtr;

    class ExprAST {
    public:
//        virtual Value *codegen() = 0;
        virtual ~ExprAST() = default;
    };

    class IntAST : public ExprAST {
    private:
        int value;
    public:
        explicit IntAST(int num) : value(num) {}
    };

    class CharAST : public ExprAST {
    private:
        uint8_t value;
    public:
        explicit CharAST(uint8_t ch) : value(ch) {}
    };

    class VariableAST : public ExprAST {
    private:
        std::string value;
    public:
        explicit VariableAST(std::string name) : value(std::move(name)) {}

        std::string getName() { return value; }
    };

}   // RJIT::AST

#endif //RJIT_AST_H
