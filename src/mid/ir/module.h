#ifndef RJIT_MODULE_H
#define RJIT_MODULE_H

#include <stack>

#include "ssa.h"
#include "lib/debug.h"
#include "lib/guard.h"
#include "lib/nestedmap.h"
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
  UserList                     _global_vars;
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

  FuncPtr CreateFunction(const std::string &name, const TYPE::TypeInfoPtr &type);

  BlockPtr CreateBlock(const UserPtr &parent);

  BlockPtr CreateBlock(const UserPtr &parent, const std::string &name);

  SSAPtr   CreateJump(const BlockPtr &target);

  UserPtr GetFunction(const std::string &func_name);

  // setters
  // set current context (logger)
  Guard SetContext(const front::Logger &logger);
  // set current context (pointer to logger)
  Guard SetContext(const front::LoggerPtr &logger);
  void SetInsertPoint(const BlockPtr &BB) { _insert_point = BB; }

  // getters
  UserList        &GlobalVars()  { return _global_vars;  }
  ValueEnvPtr     &ValueSymTab() { return _value_symtab; }
  FunctionList    &Functions()   { return _functions;    }
  BlockPtr        &InsertPoint() { return _insert_point; }
};


}

#endif //RJIT_MODULE_H
