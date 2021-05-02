#ifndef RJIT_MODULE_H
#define RJIT_MODULE_H

#include "value.h"
#include "lib/debug.h"
#include "lib/nestedmap.h"

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
  UserList        _global_vars;
  ValueEnvPtr     _value_symtab;
  FunctionList    _functions;
public:

  UserPtr GetFunction(const std::string &func_name);

  // getters
  UserList        &GlobalVars()  { return _global_vars;  }
  ValueEnvPtr     &ValueSymTab() { return _value_symtab; }
  FunctionList    &Functions()   { return _functions;    }
};

}

#endif //RJIT_MODULE_H
