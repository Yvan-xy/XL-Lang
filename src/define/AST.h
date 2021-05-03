#ifndef RJIT_AST_H
#define RJIT_AST_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "define/type.h"
#include "front/logger.h"
#include "mid/ir/usedef/value.h"

namespace RJIT::mid {
class Dumper;

class IRBuilder;

namespace analyzer {
class SemAnalyzer;
}
}


namespace RJIT::AST {
using TYPE::TypeInfoPtr;
using mid::SSAPtr;

enum class Operator {
  // unary
  Not, Neg, LNot,

  // arithmetic
  Add, Sub, Mul, SDiv, UDiv, SRem, URem, And, Or, Xor, Shl, AShr, LShr,

  // logical
  LAnd, LOr, Equal, NotEqual, Less, LessEq, Great, GreatEq,

  // assign with arithmetic
  Assign, AssAdd, AssSub, AssMul, AssSDiv, AssUDiv, AssSRem, AssURem,
  AssAnd, AssOr, AssXor, AssShl, AssAShr, AssLShr,
  Dam
};

Operator string2Operator(const std::string &op, bool isUnary = false);

std::string operator2String(Operator oper);

class BaseAST;
class Decl;
class Stmt;

typedef std::unique_ptr<BaseAST> ASTPtr;
typedef std::vector<ASTPtr> ASTPtrList;
typedef std::unique_ptr<Decl> DeclPtr;
typedef std::unique_ptr<Stmt> StmtPtr;

class BaseAST {
private:
  front::LoggerPtr logger;
  TypeInfoPtr ast_type;

public:
  virtual ~BaseAST() = default;

  void setLogger(front::LoggerPtr logger_) { logger = std::move(logger_); }

  const TypeInfoPtr &set_ast_type(const TypeInfoPtr &ast_type_) {
    return ast_type = ast_type_;
  }

  front::LoggerPtr &Logger() { return logger; }

  virtual std::string getTypeStr() { return type2String(ast_type->GetType()); }

  virtual void Dump(mid::Dumper *) = 0;

  virtual TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) = 0;

  virtual SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) = 0;

  virtual std::string ArgName() const { return ""; }

  virtual bool hasInit() { return false; }

  virtual BaseAST* getInitValue() { return nullptr; }

  const TypeInfoPtr &AstType() const { return ast_type; }
};

class Stmt : public BaseAST {
};

class Decl : public BaseAST {
};

class TranslationUnitDecl : public Decl {
private:
  ASTPtrList decls;

public:
  explicit TranslationUnitDecl(ASTPtrList _decls) : decls(std::move(_decls)) {}

  ASTPtrList &Decls() { return decls; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};


class IntAST : public BaseAST {
private:
  int value;

public:
  explicit IntAST(int num) : value(num) {}

  int getValue() const { return value; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};


class CharAST : public BaseAST {
private:
  uint8_t value;

public:
  explicit CharAST(uint8_t ch) : value(ch) {}

  uint8_t getValue() const { return value; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class StringAST : public BaseAST {
private:
  std::string value;

public:
  explicit StringAST(std::string name) : value(std::move(name)) {}

  const std::string &getValue() const { return value; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class VariableAST : public BaseAST {
private:
  std::string name;

public:
  explicit VariableAST(std::string name_) : name(std::move(name_)) {}

  const std::string &getName() const { return name; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class PrimTypeAST : public BaseAST {
private:
  using Type = RJIT::TYPE::Type;
  Type type;

public:
  explicit PrimTypeAST(Type type_) : type(type_) {}

  Type getType() const { return type; }

  std::string getTypeStr() override { return TYPE::type2String(type); }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

typedef std::unique_ptr<PrimTypeAST> PrimASTPtr;

class VariableDecl : public Decl {
private:
  PrimASTPtr type;
  ASTPtrList defs;

public:
  VariableDecl(PrimASTPtr type_, ASTPtrList defs_) : type(std::move(type_)), defs(std::move(defs_)) {}

  ASTPtrList &getDefs() { return defs; }

  PrimASTPtr &getType() { return type; }

  TYPE::Type getPrimeType() const { return type->getType(); }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class VariableDefAST : public BaseAST {
private:
  RJIT::TYPE::Type type;
  std::string identifier;
  ASTPtr initValue;

public:
  VariableDefAST(std::string id, ASTPtr init) : identifier(std::move(id)), initValue(std::move(init)) {}

  const std::string &getIdentifier() { return identifier; }

  bool hasInit() override {
    if (initValue) {
      return true;
    }
    return false;
  }

  BaseAST *getInitValue() override {return initValue.get(); }

  void setType(RJIT::TYPE::Type _type) { _type = type; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

// Binary statement
class BinaryStmt : public Stmt {
private:
  std::string _op_str;
  Operator _op;
  ASTPtr LHS, RHS;

public:
  BinaryStmt(Operator op_, ASTPtr lhs, ASTPtr rhs) : _op(op_), LHS(std::move(lhs)), RHS(std::move(rhs)) {
    _op_str = operator2String(_op);
  }

  ASTPtr &getLHS() { return LHS; }

  ASTPtr &getRHS() { return RHS; }

  const std::string &getOpStr() const { return _op_str; }

  Operator getOp() const { return _op; }
  void setOp(Operator op) { _op = op; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class UnaryStmt : public Stmt {
private:
  Operator _op;
  std::string _op_str;
  ASTPtr _operand;

public:
  UnaryStmt(Operator op, ASTPtr operand_) : _op(op), _operand(std::move(operand_)) {
    _op_str = operator2String(op);
  }

  const std::string &getOpString() { return _op_str; }

  Operator op() const { return _op; }

  ASTPtr &Operand() { return _operand; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class ReturnStmt : public Stmt {
private:
  ASTPtr retVal;

public:
  explicit ReturnStmt(ASTPtr retval) : retVal(std::move(retval)) {}

  ASTPtr &getReturn() { return retVal; }

  bool hasReturnVal() const { return retVal != nullptr; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

// statement block
class CompoundStmt : public Stmt {
private:
  ASTPtrList _stmts;

public:
  explicit CompoundStmt(ASTPtrList stmts_) : _stmts(std::move(stmts_)) {}

  const ASTPtrList &stmts() const { return _stmts; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

// if-else statement
class IfElseStmt : public Stmt {
private:
  ASTPtr _condition;
  ASTPtr _then;
  ASTPtr _else;

public:
  IfElseStmt(ASTPtr cond, ASTPtr then, ASTPtr _else)
      : _condition(std::move(cond)), _then(std::move(then)), _else(std::move(_else)) {}

  ASTPtr &getCondition() { return _condition; }

  ASTPtr &getThen() { return _then; }

  ASTPtr &getElse() { return _else; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class WhileStmt : public Stmt {
private:
  ASTPtr condition, block;

public:
  WhileStmt(ASTPtr condition_, ASTPtr block_) : condition(std::move(condition_)), block(std::move(block_)) {}

  ASTPtr &getCondition() { return condition; }

  ASTPtr &getBlock() { return block; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class CallStmt : public Stmt {
private:
  std::string symbol;
  ASTPtrList args;

public:
  CallStmt(std::string symbol_, ASTPtrList args_) : symbol(std::move(symbol_)), args(std::move(args_)) {}

  const std::string &getSymbol() { return symbol; }

  ASTPtrList &getArgs() { return args; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class BreakStmt : public Stmt {
public:
  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class ContinueStmt : public Stmt {
public:
  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class ProtoTypeAST : public BaseAST {
private:
  std::string _funcName;
  ASTPtrList  _args;
  TYPE::Type  _ret_type;
  std::string _type_str;

public:
  ProtoTypeAST(std::string name, ASTPtrList args_, TYPE::Type type_) :
      _funcName(std::move(name)), _args(std::move(args_)), _ret_type(type_) {
    _type_str = TYPE::type2String(_ret_type);
  }

  std::string getFuncName() { return _funcName; }

  ASTPtrList &getArgs() { return _args; }

  const std::string &getReturnTypeStr() const { return _type_str; }

  TYPE::Type getReturnType() const { return _ret_type; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class FunctionDefAST : public Decl {
private:
  ASTPtr protoType;
  ASTPtr body;

public:
  FunctionDefAST(ASTPtr proto, ASTPtr body) : protoType(std::move(proto)), body(std::move(body)) {}

  ASTPtr &getProtoType() { return protoType; }

  ASTPtr &getBody() { return body; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

class FuncParamAST : public BaseAST {
private:
  ASTPtr type;
  std::string _arg_name;

public:
  FuncParamAST(ASTPtr type_, std::string id)
      : type(std::move(type_)), _arg_name(std::move(id)) {}

  std::string getTypeStr() override { return type->getTypeStr(); }

  ASTPtr &getType() { return type; }

  std::string ArgName() const override { return _arg_name; }

  void Dump(mid::Dumper *) override;

  TypeInfoPtr SemAnalyze(mid::analyzer::SemAnalyzer *analyzer) override;

  SSAPtr CodeGeneAction(mid::IRBuilder *irbuilder) override;
};

template<typename T, typename... Args>
ASTPtr MakeAST(front::LoggerPtr logger, Args &&... args) {
  auto ast = std::make_unique<T>(std::forward<Args>(args)...);
  ast->setLogger(std::move(logger));
  return ast;
}

PrimASTPtr MakePrimeAST(front::LoggerPtr logger, TYPE::Type type);

}// namespace RJIT::AST

#endif//RJIT_AST_H
