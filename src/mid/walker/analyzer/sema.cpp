#include "sema.h"
#include "lib/guard.h"
#include "lib/debug.h"
#include <utility>

namespace RJIT::mid::analyzer {

  Guard SemAnalyzer::NewEnv() {
    _symbol = MakeNestedMap(_symbol);
    _alias = MakeNestedMap(_alias);
    return Guard([this] {
      _symbol = _symbol->outer();
      _alias = _alias->outer();
    });
  }

  TypeInfoPtr SemAnalyzer::visit(IntAST *node) {
    // make right value 'int32' type
    return node->set_ast_type(MakeConst(Type::Int32, true));
  }

  TypeInfoPtr SemAnalyzer::visit(CharAST *node) {
    // make right value 'uint8' type as a char
    return node->set_ast_type(MakeConst(Type::UInt8, true));
  }

  TypeInfoPtr SemAnalyzer::visit(StringAST *node) {
    return node->set_ast_type(MakeConst(Type::String, true));
  }

  TypeInfoPtr SemAnalyzer::visit(VariableAST *node) {
    auto type_ = _symbol->GetItem(node->getName());
    std::string info = "undefined symbol";
    if (!type_) return LogError(node->Logger(), info, node->getName());
    return node->set_ast_type(type_);
  }

  TypeInfoPtr SemAnalyzer::visit(VariableDecl *node) {
    auto type = node->getType()->SemAnalyze(this);
    std::string info = "variable can not be void type";
    if (!type) return nullptr;
    if (type->IsVoid()) return LogError(node->Logger(), info);
    if (type->GetType() == TYPE::Type::Dam) {
      info = "declare type not found";
      return LogError(node->Logger(), info);
    }

    // check variable defines
    this->_decl_type = node->getPrimeType();
    for (const auto &i : node->getDefs()) {
      if (!i->SemAnalyze(this)) return nullptr;
    }

    return node->set_ast_type(MakePrimType(node->getPrimeType(), false));
  }

  TypeInfoPtr SemAnalyzer::visit(VariableDefAST *node) {
    std::string info;

    // check if redeclared
    if (_symbol->GetItem(node->getIdentifier(), false)) {
      info = "variable has been defined";
      return LogError(node->Logger(), info, node->getIdentifier());
    }

    TypeInfoPtr var_type;

    // if has initial value then check type
    if (node->getInitValue()) {
      auto type = node->getInitValue()->SemAnalyze(this);
      if (!type) return nullptr;

      if (_decl_type != type->GetType()) {
        info = "init value type should be " + TYPE::type2String(_decl_type);
        return LogError(node->Logger(), info, node->getIdentifier());
      }
    }
    node->setType(_decl_type);

    // Add to environment
    var_type = MakePrimType(_decl_type, false);
    _symbol->AddItem(node->getIdentifier(), var_type);

    return node->set_ast_type(MakePrimType(node->type(), false));
  }

  TypeInfoPtr SemAnalyzer::visit(BinaryStmt *node) {
    using AST::Operator;
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

    if (node->getOp() <= Operator::SGreatEq) {
      if (lhs->IsInteger() && rhs->IsInteger()) {
        if (lhs->GetSize() != rhs->GetSize()) {
          type = lhs->GetSize() > rhs->GetSize() ? lhs : rhs;
        } else {
          type = lhs;
        }
      }
    }

    // update operator type
    auto originOp = node->getOp();
    if (originOp == Operator::SDiv    || originOp == Operator::SRem    ||
        originOp == Operator::AShr    || originOp == Operator::AssAShr ||
        originOp == Operator::SLess   || originOp == Operator::SGreat  ||
        originOp == Operator::AssSDiv || originOp == Operator::AssSRem ||
        originOp == Operator::SLessEq || originOp == Operator::SGreatEq) {
      // TODO: here
      if (lhs->IsUnsigned() || rhs->IsUnsigned()) {
        Operator op = Operator::Dam;
        switch (originOp) {
          case Operator::SDiv:     op = Operator::UDiv;     break;
          case Operator::SRem:     op = Operator::URem;     break;
          case Operator::SLess:    op = Operator::ULess;    break;
          case Operator::SGreat:   op = Operator::UGreat;   break;
          case Operator::AssSDiv:  op = Operator::AssUDiv;  break;
          case Operator::AssSRem:  op = Operator::AssURem;  break;
          case Operator::SLessEq:  op = Operator::ULessEq;  break;
          case Operator::SGreatEq: op = Operator::UGreatEq; break;
          case Operator::AShr: {
            if (lhs->IsUnsigned()) op = Operator::LShr;
            break;
          }
          case Operator::AssAShr: {
            if (lhs->IsUnsigned()) op = Operator::AssLShr;
            break;
          }
          default: break;
        }
        node->setOp(op);
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

  TypeInfoPtr SemAnalyzer::visit(UnaryStmt *node) {
    std::string info;
    auto operand_type = node->Operand()->SemAnalyze(this);
    if (!operand_type->IsInteger() && !operand_type->IsBool()) {
      info = "unary operand is not integer or bool type";
      return LogError(node->Logger(), info);
    }

    if (node->op() == Operator::Dam) {
      info = "unary operator is Dam";
      return LogError(node->Logger(), info);
    }

    return node->set_ast_type(operand_type);
  }

  TypeInfoPtr SemAnalyzer::visit(ReturnStmt *node) {
    std::string info;
    TypeInfoPtr type;
    if (node->hasReturnVal()) {
      type = node->getReturn()->SemAnalyze(this);
    } else {
      type = MakeVoid();
    }
    if (!type) return nullptr;

    if (_ret_type == TYPE::Type::Void && !type->IsVoid()) {
      info = "function should not have return value";
      return LogError(node->Logger(), info);
    }

    auto prim_type = type->GetType();
    if (_ret_type != prim_type) {
      if (type->IsInteger() &&
          (_ret_type >= TYPE::Type::Int8 && _ret_type <= TYPE::Type::UInt32)) {
        type = MakePrimType(_ret_type, true);
      }
    } else if (_ret_type == prim_type) {
      return type = MakePrimType(_ret_type, true);
    }
    info = "return value's type is different from prototype";
    return LogError(node->Logger(), info);
  }

  TypeInfoPtr SemAnalyzer::visit(BreakStmt *node) {
    return nullptr;
  }

  TypeInfoPtr SemAnalyzer::visit(ContinueStmt *node) {
    return nullptr;
  }

  TypeInfoPtr SemAnalyzer::visit(CompoundStmt *node) {
    // make new environment when not in function
    auto guard = !_in_func ? NewEnv() : Guard(nullptr);
    if (_in_func) _in_func = false;

    for (auto &i : node->stmts()) {
      auto type = i->SemAnalyze(this);
      if (!type) return nullptr;
    }
    return node->set_ast_type(MakeVoid());
  }

  TypeInfoPtr SemAnalyzer::visit(IfElseStmt *node) {
    auto cond_type = node->getCondition()->SemAnalyze(this);
    auto then_type = node->getThen()->SemAnalyze(this);
    if (node->hasElse())
      auto else_type = node->getElse()->SemAnalyze(this);
    return node->set_ast_type(MakeVoid());
  }

  // TODO: check parameter type
  TypeInfoPtr SemAnalyzer::visit(CallStmt *node) {
    std::string info;

    // check return type here
    auto callee = _symbol->GetItem(node->getSymbol());
    if (callee == nullptr) {
      info = "function '" + node->getSymbol() + "' undefined";
      return LogError(node->Logger(), info);
    }
    auto ret_typeinfo = callee->GetReturnType();

    // check parameter reference here
    TypePtrList real_args;
    for (const auto &it : node->getArgs()) {
      auto real_arg_type = it->SemAnalyze(this);
      real_args.push_back(real_arg_type);
    }

    // type check
    auto args = callee->GetArgsType();
    if (args) {
      if (!args->empty()) {
        if (args->size() != real_args.size()) {
          info = "function '" + node->getSymbol() + "' need " + std::to_string(args->size());
          info += " parameters, but given " + std::to_string(real_args.size()) + " values.";
          return LogError(node->Logger(), info, node->getSymbol());
        }

        auto args_types = args.value();
        for (std::size_t i = 0; i < args_types.size(); i++) {
          if (args_types[i]->GetTypeId() != real_args[i]->GetTypeId()) {
            info = "function '" + node->getSymbol() + "(";
            auto tail = --(args_types.end());
            for (const auto &it : args_types) {
              info += type2String(it->GetType());
              if (it != *tail) info += ", ";
            }
            info += ")' parameter types are incompliant.";
            return LogError(node->Logger(), info, node->getSymbol());
          }
        }
      } else {
        if (!node->getArgs().empty()) {
          info = "function don't need parameters here";
          return LogError(node->Logger(), info, node->getSymbol());
        }
      }
    }

    return node->set_ast_type(MakePrimType(ret_typeinfo->GetType(), ret_typeinfo->IsRightValue()));
  }

  TypeInfoPtr SemAnalyzer::visit(ProtoTypeAST *node) {
    std::string info;
    Type ret = node->getReturnType();
    if (ret == TYPE::Type::Dam) return nullptr;
    if (_in_func) this->_ret_type = ret;

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

    const auto &sym = _in_func ? _symbol->outer() : _symbol;
    if (sym->GetItem(node->getFuncName(), false)) {
      info = "symbol has already been defined";
      return LogError(node->Logger(), info, node->getFuncName());
    }
    sym->AddItem(node->getFuncName(), type);


    return node->set_ast_type(type);
  }

  TypeInfoPtr SemAnalyzer::visit(FunctionDefAST *node) {
    auto guard = NewEnv();
    // set flag, this flag will be cleared when entering body
    _in_func = true;

    auto protoType = node->getProtoType()->SemAnalyze(this);
    auto body = node->getBody()->SemAnalyze(this);
    if (!protoType || !body) return nullptr;
    return node->set_ast_type(protoType);
  }

  TypeInfoPtr SemAnalyzer::visit(FuncParamAST *node) {
    std::string info;
    auto type = node->getType()->SemAnalyze(this);
    if (!type) return nullptr;

    if (_in_func) {
      // check if is conflicted
      if (_symbol->GetItem(node->ArgName(), false)) {
        info = "argument has already been declared";
        return LogError(node->Logger(), info, node->ArgName());
      }
      _symbol->AddItem(node->ArgName(), type);
    }

    type = MakePrimType(type->GetType(), true);
    return node->set_ast_type(type);
  }

  TypeInfoPtr SemAnalyzer::visit(PrimTypeAST *node) {
    return node->set_ast_type(MakePrimType(node->getType(), false));
  }

  TypeInfoPtr SemAnalyzer::visit(WhileStmt *node) {
    return nullptr;
  }

  TypeInfoPtr SemAnalyzer::visit(TranslationUnitDecl *node) {
    auto guard = !_in_func ? NewEnv() : Guard(nullptr);

    std::string info = "occur error in global definition";
    for (const auto &it : node->Decls()) {
      auto type = it->SemAnalyze(this);
      if (type == nullptr){}
//        break;
//        LogError(node->Logger(), info, "TranslationDeclUnit");
    }
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