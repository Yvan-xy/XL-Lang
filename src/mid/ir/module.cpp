#include "ssa.h"
#include "module.h"

namespace RJIT::mid {

UserPtr Module::GetFunction(const std::string &func_name) {
  for (const auto &it : _functions) {
    it->GetFunctionName();
    if (it->GetFunctionName() == func_name) {
      return it;
    }
  }
  return nullptr;
}

}