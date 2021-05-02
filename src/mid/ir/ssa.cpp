#include "ssa.h"

namespace RJIT::mid {

Instruction::Instruction(unsigned opcode, unsigned operand_nums, const SSAPtr& insertBefore)
    : User(operand_nums), _opcode(opcode) {
  if (insertBefore != nullptr) {
    DBG_ASSERT(insertBefore->GetParent(), "InsertBefore do not contain a parent(BasicBlock)");
    insertBefore->GetParent()->AddInstBefore(insertBefore, SSAPtr(this));
  }
}

Instruction::Instruction(unsigned opcode, unsigned operand_nums,
   const Operands& operands, const SSAPtr& insertBefore)
   : User(operand_nums, operands), _opcode(opcode) {
  if (insertBefore != nullptr) {
    DBG_ASSERT(insertBefore->GetParent(), "InsertBefore do not contain a parent(BasicBlock)");
    auto insts = insertBefore->GetParent()->insts();
    auto it = std::find(insts.begin(), insts.end(), insertBefore);
    insts.insert(it, SSAPtr(this));
  }
}

Instruction::Instruction(unsigned opcode, unsigned operand_nums, const BlockPtr &insertAtEnd)
    : User(operand_nums), _opcode(opcode) {
  DBG_ASSERT(insertAtEnd != nullptr, "Basic block to append to may not be NULL");
  insertAtEnd->AddInstToEnd(SSAPtr(this));
}

Instruction::Instruction(unsigned opcode, unsigned operand_nums,
            const Operands& operands, const BlockPtr &insertAtEnd)
    : User(operand_nums, operands), _opcode(opcode) {
  DBG_ASSERT(insertAtEnd != nullptr, "Basic block to append to may not be NULL");
  insertAtEnd->AddInstToEnd(SSAPtr(this));
}


void Instruction::SetParent(const BlockPtr& B) {
  DBG_ASSERT(GetParent() == nullptr, "SetParent() instruction parent is not NULL");
  _parent = B;
}

std::string Instruction::GetOpcodeAsString(unsigned int opcode) const {
    switch (opcode) {
      // Terminators
      case Ret:    return "ret";
      case Br:     return "br";

        // Standard binary operators...
      case Add:  return "add";
      case Sub:  return "sub";
      case Mul:  return "mul";
      case UDiv: return "udiv";
      case SDiv: return "sdiv";
//      case FDiv: return "fdiv";
      case URem: return "urem";
      case SRem: return "srem";
//      case FRem: return "frem";

        // Logical operators...
      case And: return "and";
      case Or : return "or";
      case Xor: return "xor";

        // Memory instructions...
      case Malloc:        return "malloc";
      case Free:          return "free";
      case Alloca:        return "alloca";
      case Load:          return "load";
      case Store:         return "store";
//      case GetElementPtr: return "getelementptr";

        // Convert instructions...
      case Trunc:     return "trunc";
      case ZExt:      return "zext";
      case SExt:      return "sext";

#if 0
      case FPTrunc:   return "fptrunc";
      case FPExt:     return "fpext";
      case FPToUI:    return "fptoui";
      case FPToSI:    return "fptosi";
      case UIToFP:    return "uitofp";
      case SIToFP:    return "sitofp";
#endif
      case IntToPtr:  return "inttoptr";
      case PtrToInt:  return "ptrtoint";
      case BitCast:   return "bitcast";

        // Other instructions...
      case ICmp:           return "icmp";
//      case FCmp:           return "fcmp";
      case PHI:            return "phi";
      case Select:         return "select";
      case Call:           return "call";
      case Shl:            return "shl";
      case LShr:           return "lshr";
      case AShr:           return "ashr";
      case VAArg:          return "va_arg";
      case ExtractElement: return "extractelement";
      case InsertElement:  return "insertelement";
      case ShuffleVector:  return "shufflevector";

      default: return "<Invalid operator> ";
    }
  return "";
}

//===----------------------------------------------------------------------===//
//                           BinaryOperator Class
//===----------------------------------------------------------------------===//

BinaryOperator::BinaryOperator(Instruction::BinaryOps opcode, const SSAPtr &S1,
   const SSAPtr &S2, const SSAPtr &IB)
   : Instruction(opcode, 2, IB) {
  AddValue(S1);
  AddValue(S2);
}

BinaryOperator::BinaryOperator(Instruction::BinaryOps opcode, const SSAPtr &S1,
   const SSAPtr &S2, const BlockPtr &IAE)
   : Instruction(opcode, 2, IAE){
  AddValue(S1);
  AddValue(S2);
}

BinaryPtr
BinaryOperator::Create(Instruction::BinaryOps opcode, const SSAPtr &S1,
     const SSAPtr &S2, const SSAPtr &IB) {
  auto s1_type = S1->type();
  auto s2_type = S2->type();
  DBG_ASSERT(s1_type->IsPrime(), "S1 is not prime type");
  DBG_ASSERT(s2_type->IsPrime(), "S2 is not prime type");

  DBG_ASSERT(s1_type == s2_type, "S1 has different type with S2");
  return std::make_shared<BinaryOperator>(opcode, S1, S2, IB);
}

BinaryPtr
BinaryOperator::Create(Instruction::BinaryOps opcode, const SSAPtr &S1,
     const SSAPtr &S2, const BlockPtr &IAE) {
  auto s1_type = S1->type();
  auto s2_type = S2->type();
  DBG_ASSERT(s1_type->IsPrime(), "S1 is not prime type");
  DBG_ASSERT(s2_type->IsPrime(), "S2 is not prime type");

  DBG_ASSERT(s1_type == s2_type, "S1 has different type with S2");
  return std::make_shared<BinaryOperator>(opcode, S1, S2, IAE);

}

void BasicBlock::AddInstBefore(const SSAPtr &insertBefore, const SSAPtr &inst) {
  auto it = std::find(_insts.begin(), _insts.end(), insertBefore);
  DBG_ASSERT(it != _insts.end(), "Basic block don't has this instruction");
  _insts.insert(it, inst);
}

}