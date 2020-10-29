#ifndef RJIT_AST_H
#define RJIT_AST_H

#include <utility>
#include <vector>
#include <memory>
#include <utility>

#include "define/type.h"

namespace RJIT::AST {
    enum class Operator {
        Add, Sub, Mul, Div, Mod,
        And, Or, Xor, Shl, Shr,
        LAnd, LOr,
        Equal, NotEqual, Less, LessEq, Great, GreatEq,
        Assign,
        AssAdd, AssSub, AssMul, AssDiv, AssMod,
        AssAnd, AssOr, AssXor, AssShl, AssShr,
    };

    class ExprAST {
    public:
//        virtual Value *codegen() = 0;
        virtual ~ExprAST() = default;
    };

    typedef std::unique_ptr<ExprAST> ASTPtr;
    typedef std::vector<ASTPtr> ASTPtrList;

    class IntAST : public ExprAST {
    private:
        int value;
    public:
        explicit IntAST(int num) : value(num) {}

        int getValue() const { return value; }
    };

    class CharAST : public ExprAST {
    private:
        uint8_t value;
    public:
        explicit CharAST(uint8_t ch) : value(ch) {}

        uint8_t getValue() const { return value; }
    };

    class StringAST : public ExprAST {
    private:
        std::string value;
    public:
        explicit StringAST(std::string name) : value(std::move(name)) {}

        const std::string &getValue() const { return value; }
    };

    class VariableAST : public ExprAST {
    private:
        std::string name;
    public:
        explicit VariableAST(std::string name_) : name(std::move(name_)) {}

        const std::string &getName() const { return name; }
    };

    class VariableDeclAST : public ExprAST {
    private:
        ASTPtr type;
        ASTPtrList defs;
    public:
        VariableDeclAST(ASTPtr type_, ASTPtrList defs_) :
                type(std::move(type_)), defs(std::move(defs_)) {}
    };

    class VariableDefAST : public ExprAST {
    private:
        std::string identifier;
        ASTPtr initValue;
    public:
        VariableDefAST(std::string id, ASTPtr init) :
                identifier(std::move(id)), initValue(std::move(init)) {}
    };

    // Binary statement
    class BinaryAST : public ExprAST {
    private:
        Operator op;
        ASTPtr LHS, RHS;
    public:
        BinaryAST(Operator op_, ASTPtr lhs, ASTPtr rhs) :
                op(op_), LHS(std::move(lhs)), RHS(std::move(rhs)) {}

    };

    class UnaryAST : public ExprAST {
    private:
        std::string oper;
        ASTPtr operand;
    public:
        UnaryAST(std::string operator_, ASTPtr operand_) :
                oper(std::move(operator_)), operand(std::move(operand_)) {}
    };

    // statement block
    class BlockAST : public ExprAST {
    private:
        ASTPtrList stmts;
    public:
        explicit BlockAST(ASTPtrList stmts_) : stmts(std::move(stmts_)) {}

        const ASTPtrList &getStmts() const { return stmts; }
    };

    // if-else statement
    class IfElseAST : public ExprAST {
        ASTPtr condition, then_, else_;
    public:
        IfElseAST(ASTPtr cond, ASTPtr then, ASTPtr _else) :
                condition(std::move(cond)), then_(std::move(then)), else_(std::move(_else)) {}

    };

    class CallAST : public ExprAST {
    private:
        std::string symbol;
        ASTPtrList args;
    public:
        CallAST(std::string symbol_, ASTPtrList args_) :
                symbol(std::move(symbol_)), args(std::move(args_)) {}

    };

    class ProtoTypeAST {
    private:
        std::string funcName;
        ASTPtrList args;
    public:

        ProtoTypeAST(std::string name, ASTPtrList args_) :
                funcName(std::move(name)), args(std::move(args_)) {}

        std::string getFuncName() { return funcName; }
    };

    typedef std::unique_ptr<ProtoTypeAST> ProtoPtr;

    class FunctionDefAST {
    private:
        ProtoPtr protoType;
        ASTPtr body;
    public:
        FunctionDefAST(ProtoPtr proto, ASTPtr body) :
                protoType(std::move(proto)), body(std::move(body)) {}
    };

    typedef std::unique_ptr<FunctionDefAST> FuncPtr;

    class FuncParamAST : public ExprAST {
    private:
        ASTPtr type;
        std::string identifier;
    public:
        FuncParamAST(ASTPtr type_, std::string id) :
                type(std::move(type_)), identifier(std::move(id)) {}
    };


    class PrimTypeAST : public ExprAST {
    private:
        using Type = RJIT::TYPE::Type;
        Type type;
    public:
    };


}   // RJIT::AST

#endif //RJIT_AST_H
