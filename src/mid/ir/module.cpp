#include <memory>
#include "module.h"
#include "constant.h"
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

FuncPtr Module::GetFunction(const std::string &func_name) {
  for (const auto &it : _functions) {
    it->GetFunctionName();
    if (it->GetFunctionName() == func_name) {
      return it;
    }
  }
  return nullptr;
}

SSAPtr Module::GetValues(const std::string &var_name) {
  return _value_symtab->GetItem(var_name);
}

BlockPtr Module::CreateBlock(const UserPtr &parent) {
  return CreateBlock(parent, "block");
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
  target->AddValue(_insert_point);
  return jump;
}

// TODO: add necessary cast
SSAPtr Module::CreateReturn(const SSAPtr &value) {
  auto ret = AddInst<ReturnInst>(value);
  ret->set_type(nullptr);
  return ret;
}

SSAPtr Module::CreateBranch(const SSAPtr &cond, const BlockPtr &true_block,
                            const BlockPtr &false_block) {
  // check condition type
  DBG_ASSERT(cond->type()->IsInteger(), "cond type should be integer");

  // create branch instruction
  auto br = AddInst<BranchInst>(cond, true_block, false_block);
  br->set_type(nullptr);

  // update predecessor
  true_block->AddValue(_insert_point);
  false_block->AddValue(_insert_point);
  return br;
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
  auto last_pos = _func_entry->inst_end();
  SetInsertPoint(_func_entry, --last_pos);

  // create alloca and insert it to entry
  DBG_ASSERT(!type->IsVoid(), "alloc type can't be void");
  auto alloca = AddInst<AllocaInst>();
  auto ptr_type = TYPE::MakePointerType(type);
  alloca->set_type(ptr_type);

  // recover insert point to previous block
  SetInsertPoint(insert_point);
  return alloca;
}

SSAPtr Module::CreateLoad(const SSAPtr &ptr) {
  auto type = ptr->type();
  DBG_ASSERT(type->IsPointer(), "loading from non-pointer type is forbidden");
  auto load = AddInst<LoadInst>(ptr);

  // set load type
  auto load_type = ptr->type()->GetDereferenceType();
  DBG_ASSERT(load_type != nullptr, "load type is nullptr");
  load->set_type(load_type);
  return load;
}

static unsigned OpToOpcode(AST::Operator op) {
  using OtherOps  = Instruction::OtherOps;
  using BinaryOps = Instruction::BinaryOps;
  using AssignOps = Instruction::AssignOps;
  switch (op) {
    /* ----------- unary operator ------------ */
    case AST::Operator::Not:    return BinaryOps::Xor;
    case AST::Operator::Neg:    return BinaryOps::Sub;
    case AST::Operator::LNot:   return BinaryOps::And;  // operand && 1

    /* ----------- binary operator ------------ */
    case AST::Operator::Add:      return BinaryOps::Add;
    case AST::Operator::Sub:      return BinaryOps::Sub;
    case AST::Operator::Mul:      return BinaryOps::Mul;
    case AST::Operator::SDiv:     return BinaryOps::SDiv;
    case AST::Operator::UDiv:     return BinaryOps::UDiv;
    case AST::Operator::SRem:     return BinaryOps::SRem;
    case AST::Operator::URem:     return BinaryOps::URem;
    case AST::Operator::And:      return BinaryOps::And;
    case AST::Operator::Or:       return BinaryOps::Or;
    case AST::Operator::Xor:      return BinaryOps::Xor;
    case AST::Operator::Shl:      return BinaryOps::Shl;
    case AST::Operator::AShr:     return BinaryOps::AShr;
    case AST::Operator::LShr:     return BinaryOps::LShr;
    case AST::Operator::LAnd:     return BinaryOps::And;
    case AST::Operator::LOr:      return BinaryOps::Or;

    /* ----------- compare operator ------------ */
    case AST::Operator::Equal:    return OtherOps::ICmp;
    case AST::Operator::NotEqual: return OtherOps::ICmp;
    case AST::Operator::SLess:    return OtherOps::ICmp;
    case AST::Operator::ULess:    return OtherOps::ICmp;
    case AST::Operator::SGreat:   return OtherOps::ICmp;
    case AST::Operator::UGreat:   return OtherOps::ICmp;
    case AST::Operator::SLessEq:  return OtherOps::ICmp;
    case AST::Operator::ULessEq:  return OtherOps::ICmp;
    case AST::Operator::SGreatEq: return OtherOps::ICmp;
    case AST::Operator::UGreatEq: return OtherOps::ICmp;

    /* ----------- assign operator ------------ */
    case AST::Operator::Assign:   return AssignOps::Assign;
    case AST::Operator::AssAdd:   return AssignOps::AssAdd;
    case AST::Operator::AssSub:   return AssignOps::AssSub;
    case AST::Operator::AssMul:   return AssignOps::AssMul;
    case AST::Operator::AssSDiv:  return AssignOps::AssSDiv;
    case AST::Operator::AssUDiv:  return AssignOps::AssUDiv;
    case AST::Operator::AssSRem:  return AssignOps::AssSRem;
    case AST::Operator::AssURem:  return AssignOps::AssURem;
    case AST::Operator::AssAnd:   return AssignOps::AssAnd;
    case AST::Operator::AssOr:    return AssignOps::AssOr;
    case AST::Operator::AssXor:   return AssignOps::AssXor;
    case AST::Operator::AssShl:   return AssignOps::AssShl;
    case AST::Operator::AssAShr:  return AssignOps::AssAShr;
    case AST::Operator::AssLShr:  return AssignOps::AssLShr;
    case AST::Operator::Dam:      return OtherOps::Undef;
  }
  return OtherOps::Undef;
}

// S1 = S2;
SSAPtr Module::CreateAssign(const SSAPtr &S1, const SSAPtr &S2) {
  std::shared_ptr<Instruction> inst;


  if (S2->type()->IsConst() || IsBinaryOperator(S2) || IsCallInst(S2)) {
    // S1 = C ---> store C, s1
    auto store_inst = AddInst<StoreInst>(S2, S1);
    DBG_ASSERT(store_inst != nullptr, "emit store inst failed");
    return store_inst;
  } else {
    // S1 = S2 ---> %0 = load s2; store %0, i32* s1
    auto load_inst = CreateLoad(S2);

    // TODO: add necessary cast here
    auto store_inst = AddInst<StoreInst>(load_inst, S1);
    DBG_ASSERT(store_inst != nullptr, "emit store inst failed");
    return store_inst;
  }
}

SSAPtr Module::CreatePureBinaryInst(Instruction::BinaryOps opcode,
                                    const SSAPtr &S1, const SSAPtr &S2) {
  DBG_ASSERT(opcode >= Instruction::BinaryOps::Add, "opcode is not pure binary operator");
  SSAPtr load_s1 = nullptr;
  SSAPtr load_s2 = nullptr;
  if (!S1->type()->IsConst() && !IsBinaryOperator(S1) && !IsCallInst(S1)) {
    load_s1 = CreateLoad(S1);
    DBG_ASSERT(load_s1 != nullptr, "emit load S1 failed");
  }

  if (!S2->type()->IsConst() && !IsBinaryOperator(S2) && !IsCallInst(S2)) {
    load_s2 = CreateLoad(S2);
    DBG_ASSERT(load_s1 != nullptr, "emit load S2 failed");
  }

  auto bin_inst = BinaryOperator::Create(opcode,
                                         ((load_s1 != nullptr) ? load_s1 : S1),
                                         ((load_s2 != nullptr) ? load_s2 : S2));

  // set binary instruction type
  auto s1_type = S1->type();
  if (s1_type->IsPointer()) {
    bin_inst->set_type(s1_type->GetDereferenceType());
  } else {
    bin_inst->set_type(S1->type());
  }

  // add inst into basic block
  _insert_point->AddInstToEnd(bin_inst);
  DBG_ASSERT(bin_inst != nullptr, "emit binary instruction failed");
  return bin_inst;
}


SSAPtr Module::CreateBinaryOperator(AST::Operator op,
                                    const SSAPtr &S1, const SSAPtr &S2) {
  using OtherOps  = Instruction::OtherOps;
  using BinaryOps = Instruction::BinaryOps;
  using AssignOps = Instruction::AssignOps;
  auto opcode = OpToOpcode(op);

  // create assign operator
  if (opcode == AssignOps::Assign) {
    return CreateAssign(S1, S2);
  } else if (opcode >= AssignOps::AssAdd && opcode <= AssignOps::AssignOpsEnd) {
    auto bin_inst = CreatePureBinaryInst(static_cast<BinaryOps>(opcode), S1, S2);
    auto assign_inst = CreateAssign(S1, bin_inst);
    return assign_inst;
  } else if (opcode == OtherOps::ICmp) {
    return CreateICmpInst(op, S1, S2);
  } else if (opcode >= BinaryOps::Add && opcode <= BinaryOps::BinaryOpsEnd) {
    return CreatePureBinaryInst(static_cast<BinaryOps>(opcode), S1, S2);
  }


  return nullptr;
}

SSAPtr Module::CreateConstInt(unsigned int value) {
  using namespace RJIT::TYPE;
  auto const_int =  MakeSSA<ConstantInt>(value);
  const_int->set_type(MakeConst(Type::UInt32));
  DBG_ASSERT(const_int != nullptr, "emit const int value failed");
  return const_int;
}

SSAPtr Module::CreateCallInst(const SSAPtr &callee, const std::vector<SSAPtr>& args) {
  std::vector<SSAPtr> new_args;
  for (const auto &it : args) {

    if (it->type()->IsConst() || IsBinaryOperator(it) || IsCallInst(it)) {
      new_args.push_back(it);
    } else {
      auto load_inst = CreateLoad(it);
      DBG_ASSERT(load_inst != nullptr, "emit load inst before call inst failed");
      new_args.push_back(load_inst);
    }
  }

  auto call_inst = AddInst<CallInst>(callee, new_args);
  DBG_ASSERT(call_inst != nullptr, "emit call inst failed");
  call_inst->set_type(callee->type()->GetReturnType());
  return call_inst;
}

SSAPtr Module::CreateICmpInst(AST::Operator opcode, const SSAPtr &lhs, const SSAPtr &rhs) {
  DBG_ASSERT(lhs != nullptr, "lhs SSA is null ptr");
  DBG_ASSERT(rhs != nullptr, "rhs SSA is null ptr");

  SSAPtr icmp_inst, lhs_ssa, rhs_ssa;
  bool is_lhs_bin = false, is_rhs_bin = false;

  if (lhs->isInstruction()) {
    auto lhs_inst = std::static_pointer_cast<Instruction>(lhs);
    if (lhs_inst->isBinaryOp()) {
      is_lhs_bin = true;
    }
  }

  if (rhs->isInstruction()) {
    auto rhs_inst = std::static_pointer_cast<Instruction>(rhs);
    if (rhs_inst->isBinaryOp()) {
      is_rhs_bin = true;
    }
  }

  if (lhs->type()->IsConst() || is_lhs_bin) {
    lhs_ssa = lhs;
  } else {
    lhs_ssa = CreateLoad(lhs);
  }

  if (rhs->type()->IsConst() || is_rhs_bin) {
    rhs_ssa = rhs;
  } else {
    rhs_ssa = CreateLoad(rhs);
  }

  icmp_inst = AddInst<ICmpInst>(opcode, lhs_ssa, rhs_ssa);
  DBG_ASSERT(icmp_inst != nullptr, "emit ICmp instruction failed");
  icmp_inst->set_type(TYPE::MakePrimType(TYPE::Type::Bool, true));

  return icmp_inst;
}


}