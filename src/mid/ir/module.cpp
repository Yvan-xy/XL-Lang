#include <memory>
#include "module.h"

namespace RJIT::mid {

void Module::reset() {
  _global_vars.clear();
  _value_symtab.reset();
  _functions.clear();
}

Guard Module::NewEnv() {
  _value_symtab = lib::MakeNestedMap(_value_symtab);
  return Guard([this] { _value_symtab = _value_symtab->outer(); });
}

Guard Module::SetContext(const front::Logger &logger) {
  return SetContext(std::make_shared<front::Logger>(logger));
}

Guard Module::SetContext(const front::LoggerPtr &logger) {
  _loggers.push(logger);
  return Guard([this] { _loggers.pop(); });
}

FuncPtr Module::CreateFunction(const std::string &name, const TYPE::TypeInfoPtr &type) {
  DBG_ASSERT(type->IsFunction(), "not function type");
  auto func = MakeSSA<Function>(name);
  func->set_type(type);
  _functions.push_back(func);
  return func;
}

UserPtr Module::GetFunction(const std::string &func_name) {
  for (const auto &it : _functions) {
    it->GetFunctionName();
    if (it->GetFunctionName() == func_name) {
      return it;
    }
  }
  return nullptr;
}

BlockPtr Module::CreateBlock(const UserPtr &parent) {
  return CreateBlock(parent, "");
}

// TODO: fix the type cast
BlockPtr Module::CreateBlock(const UserPtr &parent, const std::string &name) {
  DBG_ASSERT((parent != nullptr) && parent->type()->IsFunction(),
             "block's parent should be function type");
  auto block = MakeSSA<BasicBlock>(parent, name);
  block->set_type(nullptr);

  // update parent function use-def info
  parent->AddValue(block);
  return block;
}

SSAPtr Module::CreateJump(const BlockPtr &target) {
  auto jump = AddInst<JumpInst>(target);
  jump->set_type(nullptr);

  // add the use to predecessor
  target->AddValue(std::static_pointer_cast<Value>(_insert_point));
  return jump;
}


}