#include <memory>
#include "module.h"
#include "idmanager.h"

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

void Module::Dump(std::ostream &os) {
  IdManager id_mgr;

  // dump global value
  for (const auto &it : _global_vars) {
    it->Dump(os, id_mgr);
  }

  // dump functions
  for (const auto &it : _functions) {
    it->Dump(os, id_mgr);
  }
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

// TODO: add necessary cast before store
SSAPtr Module::CreateStore(const SSAPtr &V, const SSAPtr &P) {
  auto store = AddInst<StoreInst>(V, P);
  store->set_type(nullptr);
  return store;
}

SSAPtr Module::CreateArgRef(const SSAPtr &func, std::size_t index, const std::string &arg_name) {
  // checking
  auto args_type = *func->type()->GetArgsType();
  DBG_ASSERT(index < args_type.size(), "index out of range");

  // set arg type
  auto arg_ref = MakeSSA<ArgRefSSA>(func, index, arg_name);
  arg_ref->set_type(args_type[index]);

  // update function
  std::dynamic_pointer_cast<Function>(func)->set_arg(index, arg_ref);
  return arg_ref;
}

SSAPtr Module::CreateAlloca(const TYPE::TypeInfoPtr &type) {
  // set insert point to entry block
  auto insert_point = _insert_point;
  SetInsertPoint(_func_entry);

  // create alloca and insert it to entry
  DBG_ASSERT(!type->IsVoid(), "alloc type can't be void");
  auto alloca = AddInst<AllocaInst>();
  auto ptr_type = TYPE::MakePointerType(type);
  alloca->set_type(ptr_type);

  // recover insert point to previous block
  SetInsertPoint(insert_point);
  return alloca;
}

// TODO: add necessary cast
SSAPtr Module::CreateReturn(const SSAPtr &value) {
  auto ret = AddInst<ReturnInst>(value);
  ret->set_type(nullptr);
  return ret;
}

SSAPtr Module::CreateLoad(const SSAPtr &ptr) {
  auto type = ptr->type();
  DBG_ASSERT(type->IsPointer(), "loading from non-pointer type is forbidden");
  auto load = AddInst<LoadInst>(ptr);
  load->set_type(ptr->type()->GetDereferenceType());
  return load;
}


}