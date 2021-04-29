#include "sema.h"
#include "lib/guard.h"
#include <utility>

namespace RJIT::mid::analyzer {

  Guard SemAnalyzer::NewEnv() {
    symbol = MakeNestedMap(symbol);
    alias = MakeNestedMap(alias);
    return Guard([this] {
      symbol = symbol->outer();
      alias = alias->outer();
    });
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(IntAST *node) {
    // make right value 'int32' type
    return node->set_ast_type(MakePrimType(Type::Int32, true));
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(CharAST *node) {
    // make right value 'uint8' type as a char
    return node->set_ast_type(MakePrimType(Type::UInt8, true));
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(StringAST *node) {
    return node->set_ast_type(MakePrimType(Type::String, true));
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(VariableAST *node) {
    auto type_ = symbol->GetItem(node->getName());
    std::string info = "undefined symbol";
    if (!type_) return LogError(node->Logger(), info, node->getName());
    return node->set_ast_type(type_);
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(VariableDecl *node) {
    auto type = node->getType()->SemAnalyze(this);
    std::string info = "variable can not be void type";
    if (!type) return nullptr;
    if (type->IsVoid()) return LogError(node->Logger(), info);
    if (type->GetType() == TYPE::Type::Dam) {
      info = "declare type not found";
      return LogError(node->Logger(), info);
    }

    // check variable defines
    this->decl_type = node->getPrimeType();
    for (const auto &i : node->getDefs()) {
      if (!i->SemAnalyze(this)) return nullptr;
    }

    return node->set_ast_type(MakeVoid());
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(VariableDefAST *node) {
    std::string info;

    // check if redeclared
    if (symbol->GetItem(node->getIdentifier(), false)) {
      info = "variable has been defined";
      return LogError(node->Logger(), info, node->getIdentifier());
    }

    TypeInfoPtr var_type;

    // if has initial value then check type
    if (node->getInitValue()) {
      auto type = node->getInitValue()->SemAnalyze(this);
      if (!type) return nullptr;

      if (decl_type != type->GetType()) {
        info = "init value type should be " + TYPE::type2String(decl_type);
        return LogError(node->Logger(), info, node->getIdentifier());
      }
    }
    node->setType(decl_type);

    // Add to environment
    var_type = MakePrimType(decl_type, false);
    symbol->AddItem(node->getIdentifier(), var_type);

    return node->set_ast_type(MakeVoid());
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(BinaryStmt *node) {
    auto lhs = node->getLHS()->SemAnalyze(this);
    auto rhs = node->getRHS()->SemAnalyze(this);
    if (!lhs || !rhs) return nullptr;

    std::string info;
    if (lhs->IsVoid() || rhs->IsVoid()) {
      info = "invalid operation between void types";
      LogError(node->Logger(), info);
    }

    if (node->getOp() == AST::Operator::Dam) {
      info = "unknown binary operator";
      LogError(node->Logger(), info);
    }

    TypeInfoPtr type = nullptr;
    bool is_right = true;

    if (node->getOp() <= AST::Operator::GreatEq) {
      if (lhs->IsInteger() && rhs->IsInteger()) {
        if (lhs->GetSize() != rhs->GetSize()) {
          type = lhs->GetSize() > rhs->GetSize() ? lhs : rhs;
        } else {
          type = lhs;
        }
      }
    }

    if (node->getOp() >= AST::Operator::Assign) {
      if (!lhs->IsRightValue() && lhs->IsInteger() && rhs->IsInteger()) {
        type = lhs;
      }
      is_right = lhs->IsRightValue();
    }

    if (!type) {
      info = "invalid binary operation";
      return LogError(node->Logger(), info);
    }
    type = MakePrimType(type->GetType(), is_right);
    return node->set_ast_type(type);
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(UnaryStmt *node) {
    return nullptr;
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(ReturnStmt *node) {
    std::string info;
    TypeInfoPtr type;
    if (node->hasReturnVal()) {
      type = node->getReturn()->SemAnalyze(this);
    } else {
      type = MakeVoid();
    }
    if (!type) return nullptr;

    if (ret_type == TYPE::Type::Void && !type->IsVoid()) {
      info = "function should not have return value";
      return LogError(node->Logger(), info);
    }

    auto prim_type = type->GetType();
    if (ret_type != prim_type) {
      if (type->IsInteger() &&
          (ret_type >= TYPE::Type::Int8 && ret_type <= TYPE::Type::UInt32)) {
        type = MakePrimType(ret_type, true);
      }
    } else if (ret_type == prim_type) {
      return type = MakePrimType(ret_type, true);
    }
    info = "return value's type is different from prototype";
    return LogError(node->Logger(), info);
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(BreakStmt *node) {
    return nullptr;
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(ContinueStmt *node) {
    return nullptr;
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(CompoundStmt *node) {
    // make new environment when not in function
    auto guard = !in_func ? NewEnv() : Guard(nullptr);
    if (in_func) in_func = false;

    for (auto &i : node->getStmts()) {
      auto type = i->SemAnalyze(this);
      if (!type) return nullptr;
    }
    return node->set_ast_type(MakeVoid());
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(IfElseStmt *node) {
    return nullptr;
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(CallStmt *node) {
    return nullptr;
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(ProtoTypeAST *node) {
    std::string info;
    Type ret = node->getReturnType();
    if (ret == TYPE::Type::Dam) return nullptr;
    if (in_func) this->ret_type = ret;

    TypePtrList params;
    for (auto &i : node->getArgs()) {
      auto param = i->SemAnalyze(this);
      if (!param) return nullptr;
      params.push_back(param);
    }

    // make function type
    auto retType = MakePrimType(ret, false);
    std::shared_ptr<FuncType> type =
        std::make_shared<FuncType>(std::move(params), std::move(retType), true);

    const auto &sym = in_func ? symbol->outer() : symbol;
    if (sym->GetItem(node->getFuncName(), false)) {
      info = "symbol has already been defined";
      return LogError(node->Logger(), info, node->getFuncName());
    }
    sym->AddItem(node->getFuncName(), type);

    return node->set_ast_type(type);
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(FunctionDefAST *node) {
    auto guard = NewEnv();
    // set flag, this flag will be cleared when entering body
    in_func = true;

    auto protoType = node->getProtoType()->SemAnalyze(this);
    auto body = node->getBody()->SemAnalyze(this);
    if (!protoType || !body) return nullptr;
    return node->set_ast_type(MakeVoid());
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(FuncParamAST *node) {
    std::string info;
    auto type = node->getType()->SemAnalyze(this);
    if (!type) return nullptr;

    if (in_func) {
      // check if is conflicted
      if (symbol->GetItem(node->getIdentifier(), false)) {
        info = "argument has already been declared";
        return LogError(node->Logger(), info, node->getIdentifier());
      }
      symbol->AddItem(node->getIdentifier(), type);
    }

    type = MakePrimType(type->GetType(), true);
    return node->set_ast_type(type);
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(PrimTypeAST *node) {
    return node->set_ast_type(MakePrimType(node->getType(), false));
  }

  TypeInfoPtr SemAnalyzer::SemAnalyze(WhileStmt *node) {
    return nullptr;
  }

  inline TypeInfoPtr LogError(const LoggerPtr &log, std::string &message, const std::string &id) {
    log->LogError(message, id);
    return nullptr;
  }

  inline TypeInfoPtr LogError(const LoggerPtr &log, std::string &message) {
    log->LogError(message);
    return nullptr;
  }

}