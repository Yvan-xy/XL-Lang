#ifndef RJIT_MODULE_H
#define RJIT_MODULE_H

#include <stack>

#include "ssa.h"
#include "lib/debug.h"
#include "lib/guard.h"
#include "lib/nestedmap.h"
#include "define/AST.h"
#include "define/type.h"

namespace RJIT::mid {

using UserList     = std::vector<UserPtr>;
using FunctionList = std::vector<FuncPtr>;
using ValueEnvPtr  = lib::Nested::NestedMapPtr<std::string, SSAPtr>;

/* Module
 * Contain all information about program.
 * Record function definitions and global variables.
 */
class Module {
private:
  SSAPtr                       _return_val;
  UserList                     _global_vars;
  BlockPtr                     _func_entry;
  BlockPtr                     _func_exit;
  BlockPtr                     _insert_point;
  ValueEnvPtr                  _value_symtab;
  FunctionList                 _functions;
  SSAPtrList::iterator         _insert_pos;
  std::stack<front::LoggerPtr> _loggers;

  // create a new SSA with current context (logger)
  template <typename T, typename... Args>
  auto MakeSSA(Args &&... args) {
    static_assert(std::is_base_of_v<Value, T>);
    auto ssa = std::make_shared<T>(std::forward<Args>(args)...);
    ssa->SetParent(_insert_point);
    ssa->set_logger(_loggers.top());
    return ssa;
  }

  // create a new instruction SSA, and push into current block
  template <typename T, typename... Args>
  auto AddInst(Args &&... args) {
    auto inst = MakeSSA<T>(std::forward<Args>(args)...);
    _insert_pos = ++_insert_point->insts().insert(_insert_pos, inst);
    return inst;
  }

public:
  Module() { reset(); }

  void reset();

  // new value env
  Guard NewEnv();

  // dump ir
  void Dump(std::ostream &os);

  FuncPtr  CreateFunction(const std::string &name, const TYPE::TypeInfoPtr &type);

  BlockPtr CreateBlock(const UserPtr &parent);

  BlockPtr CreateBlock(const UserPtr &parent, const std::string &name);

  SSAPtr   CreateJump(const BlockPtr &target);

  SSAPtr   CreateStore(const SSAPtr &V, const SSAPtr &P);

  SSAPtr   CreateArgRef(const SSAPtr &func, std::size_t index, const std::string &arg_name);

  SSAPtr   CreateAlloca(const TYPE::TypeInfoPtr &type);

  SSAPtr   CreateReturn(const SSAPtr &value);

  SSAPtr   CreateLoad(const SSAPtr &ptr);

  SSAPtr   CreateBranch(const SSAPtr &cond, const BlockPtr &true_block, const BlockPtr &false_block);

  SSAPtr   CreateBinaryOperator(AST::Operator opcode, const SSAPtr &S1, const SSAPtr &S2);

  SSAPtr   CreatePureBinaryInst(Instruction::BinaryOps opcode, const SSAPtr &S1, const SSAPtr &S2);

  SSAPtr   CreateAssign(const SSAPtr &S1, const SSAPtr &S2);

  SSAPtr   CreateConstInt(unsigned int value);

  SSAPtr   CreateCallInst(const SSAPtr &callee, const std::vector<SSAPtr>& args);

  FuncPtr GetFunction(const std::string &func_name);

  SSAPtr   GetValues(const std::string &var_name);

  // setters
  // set current context (logger)
  Guard SetContext(const front::Logger &logger);
  // set current context (pointer to logger)
  Guard SetContext(const front::LoggerPtr &logger);
  void SetRetValue   (const SSAPtr  &val)  { _return_val = val;  }
  void SetFuncEntry  (const BlockPtr &BB)  { _func_entry = BB;   }
  void SetFuncExit   (const BlockPtr &BB)  { _func_exit = BB;    }

  void SetInsertPoint(const BlockPtr &BB)  {
    SetInsertPoint(BB, BB->insts().end());
  }

  void SetInsertPoint(const BlockPtr &BB, SSAPtrList::iterator it) {
    _insert_point = BB;
    _insert_pos = it;
  }

  // getters
  SSAPtr               &ReturnValue() { return _return_val;   }
  UserList             &GlobalVars()  { return _global_vars;  }
  ValueEnvPtr          &ValueSymTab() { return _value_symtab; }
  FunctionList         &Functions()   { return _functions;    }
  BlockPtr             &InsertPoint() { return _insert_point; }
  BlockPtr             &FuncEntry()   { return _func_entry;   }
  BlockPtr             &FuncExit()    { return _func_exit;    }
  SSAPtrList::iterator  InsertPos()   { return _insert_pos;   }
};


}

#endif //RJIT_MODULE_H
