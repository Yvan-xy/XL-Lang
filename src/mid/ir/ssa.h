#ifndef RJIT_SSA_H
#define RJIT_SSA_H

#include <utility>

#include "mid/ir/usedef/user.h"

namespace RJIT::mid {

class BasicBlock : public User {
private:
  SSAPtrList _insts;
  std::string _name;    // block name
  UserPtr _parent;  // block's parent(function)

public:

  BasicBlock(UserPtr parent, std::string name)
      : _name(std::move(name)), _parent(std::move(parent)) {}

  void set_parent(const UserPtr &parent) { _parent = parent; }

  void AddInstToEnd(const SSAPtr &inst) { _insts.emplace_back(inst); }

  void AddInstBefore(const SSAPtr &insertBefore, const SSAPtr &inst);

  //getters
  SSAPtrList &insts() { return _insts; }

  const UserPtr &parent() const { return _parent; }

  auto inst_begin() const { return _insts.begin(); }

  auto inst_end() const { return _insts.end(); }
};

class Instruction : public User {
private:
  unsigned _opcode;
  BlockPtr _parent;
  InstPtr _prev, _next;

  void SetPrev(InstPtr P) { _prev = std::move(P); }

  void SetNext(InstPtr N) { _next = std::move(N); }

  void SetParent(const BlockPtr &B);

  // getNext/Prev - Return the next or previous instruction in the list.  The
  // last node in the list is a terminator instruction.
  InstPtr GetNext() { return _next; }

  const InstPtr &GetNext() const { return _next; }

  InstPtr GetPrev() { return _prev; }

  const InstPtr &GetPrev() const { return _prev; }

public:
  Instruction(unsigned opcode, unsigned operand_nums,
              const SSAPtr &insertBefore = nullptr);

  Instruction(unsigned opcode, unsigned operand_nums,
              const Operands &operands, const SSAPtr &insertBefore = nullptr);

  Instruction(unsigned opcode, unsigned operand_nums,
              const BlockPtr &insertAtEnd);

  Instruction(unsigned opcode, unsigned operand_nums,
              const Operands &operands, const BlockPtr &insertAtEnd);

  virtual ~Instruction() = default;

  // Accessor methods...
  //
  BlockPtr GetParent() override { return _parent; }

  const BlockPtr &GetParent() const { return _parent; }

  unsigned opcode() const { return _opcode; }

  std::string GetOpcodeAsString() const {
    return GetOpcodeAsString(opcode());
  }

  std::string GetOpcodeAsString(unsigned opcode) const;

  // Determine if the opcode is one of the terminators instruction.
  static inline bool isTerminator(unsigned OpCode) {
    return OpCode >= TermOpsBegin && OpCode < TermOpsEnd;
  }

  inline bool isTerminator() const {   // Instance of TerminatorInst?
    return isTerminator(opcode());
  }

  // Determine if the opcode is one of the BinaryOperator instruction.
  inline bool isBinaryOp() const {
    return opcode() >= BinaryOpsBegin && opcode() < BinaryOpsEnd;
  }

  // Determine if the Opcode is one of the shift instructions.
  static inline bool isShift(unsigned Opcode) {
    return Opcode >= Shl && Opcode <= AShr;
  }

  // Determine if the instruction's opcode is one of the shift instructions.
  inline bool isShift() const { return isShift(opcode()); }

  // isLogicalShift - Return true if this is a logical shift left or a logical shift right.
  inline bool isLogicalShift() const {
    return opcode() == Shl || opcode() == LShr;
  }

  // isLogicalShift - Return true if this is a logical shift left or a logical shift right.
  inline bool isArithmeticShift() const {
    return opcode() == AShr;
  }

  // Determine if the opcode is one of the CastInst instruction.
  static inline bool isCast(unsigned Opcode) {
    return Opcode >= CastOpsBegin && Opcode <= CastOpsEnd;
  }

  inline bool isCast() const {
    return isCast(opcode());
  }

  //----------------------------------------------------------------------
  // Exported opcode enumerations...
  // TermOps, BinaryOps, MemoryOps, CastOps, OtherOps
  //
#include "opcode.inc"
};

//===----------------------------------------------------------------------===//
//                            TerminatorInst Class
//===----------------------------------------------------------------------===//

// TerminatorInst - Subclasses of this class are all able to terminate a basic
// block.  Thus, these are all the flow control type of operations.
//
class TerminatorInst : public Instruction {
public:
  TerminatorInst(Instruction::TermOps opcode,
                 unsigned operands_num, const SSAPtr &insertBefore = nullptr)
      : Instruction(opcode, operands_num, insertBefore) {}

  TerminatorInst(Instruction::TermOps opcode, const Operands &operands,
                 unsigned operands_num, const SSAPtr &insertBefore = nullptr)
      : Instruction(opcode, operands_num, operands, insertBefore) {}

  TerminatorInst(Instruction::TermOps opcode,
                 unsigned operands_num, const BlockPtr &insertAtEnd)
      : Instruction(opcode, operands_num, insertAtEnd) {}

  TerminatorInst(Instruction::TermOps opcode, const Operands &operands,
                 unsigned operands_num, const BlockPtr &insertAtEnd)
      : Instruction(opcode, operands_num, operands, insertAtEnd) {}

  /* Virtual methods - Terminators should overload these methods. */

  // Return the number of successors that this terminator has.
  virtual unsigned GetSuccessorNum() const = 0;

  virtual SSAPtr GetSuccessor(unsigned idx) const = 0;
  virtual void SetSuccessor(unsigned idx, const BlockPtr &B) = 0;
};

//===----------------------------------------------------------------------===//
//                          UnaryInstruction Class
//===----------------------------------------------------------------------===//

class UnaryInstruction : public Instruction {
public:
  UnaryInstruction(unsigned opcode, const SSAPtr &V, const SSAPtr &IB = nullptr)
      : Instruction(opcode, 1, IB) {
    AddValue(V);
  }

  UnaryInstruction(unsigned opcode, const SSAPtr &V, const BlockPtr &IAE)
      : Instruction(opcode, 1, IAE) {
    AddValue(V);
  }


  static unsigned GetNumOperands() { return 1; }
};

//===----------------------------------------------------------------------===//
//                           BinaryOperator Class
//===----------------------------------------------------------------------===//

class BinaryOperator : public Instruction {
public:
  BinaryOperator(BinaryOps opcode, const SSAPtr &S1,
                 const SSAPtr &S2, const TYPE::TypeInfoPtr &type, const SSAPtr &IB = nullptr);

  BinaryOperator(BinaryOps opcode, const SSAPtr &S1,
                 const SSAPtr &S2, const TYPE::TypeInfoPtr &type, const BlockPtr &IAE);

  static unsigned GetNumOperands() { return 2; }

  static BinaryPtr Create(BinaryOps opcode, const SSAPtr &S1,
                          const SSAPtr &S2, const SSAPtr &IB = nullptr);

  static BinaryPtr Create(BinaryOps opcode, const SSAPtr &S1,
                          const SSAPtr &S2, const BlockPtr &IAE);

  // Create* - These methods just forward to create, and are useful when you
  // statically know what type of instruction you're going to create.  These
  // helpers just save some typing.
#define HANDLE_BINARY_INST(N, OPC, ClASS)                            \
  static BinaryPtr Create##OPC(const SSAPtr &V1, const SSAPtr &V2) { \
    return Create(Instruction::OPC, V1, V2);                         \
  }

#include "instruction.inc"

#define HANDLE_BINARY_INST(N, OPC, ClASS)                            \
  static BinaryPtr Create##OPC(const SSAPtr &V1, const SSAPtr &V2,   \
            const BlockPtr &IAE) {                                   \
    return Create(Instruction::OPC, V1, V2, IAE);                    \
  }

#include "instruction.inc"

#define HANDLE_BINARY_INST(N, OPC, ClASS)                            \
  static BinaryPtr Create##OPC(const SSAPtr &V1, const SSAPtr &V2,   \
            const SSAPtr &IB) {                                      \
    return Create(Instruction::OPC, V1, V2, IB);                     \
  }

#include "instruction.inc"


  /// Helper functions to construct and inspect unary operations (NEG and NOT)
  /// via binary operators SUB and XOR:
  ///
  /// createNeg, createNot - Create the NEG and NOT
  ///     instructions out of SUB and XOR instructions.
  ///
  static BinaryPtr createNeg(const SSAPtr &Op, const SSAPtr &InsertBefore = nullptr);
  static BinaryPtr createNeg(const SSAPtr &Op, const BlockPtr &InsertAtEnd);
  static BinaryPtr createNot(const SSAPtr &Op, const SSAPtr &InsertBefore = nullptr);
  static BinaryPtr createNot(const SSAPtr &Op, const BlockPtr &InsertAtEnd);
};

// function definition
// operands: basic blocks
class Function : public User {
private:
  std::vector<SSAPtr> _args;
  std::string _function_name;

public:
  explicit Function(std::string name) : _function_name(std::move(name)) {}

  // setters
  void set_arg(std::size_t i, const SSAPtr &arg) {
    _args.resize(i + 1);
    _args[i] = arg;
  }

  // getters
  const std::string &GetFunctionName() const { return _function_name; }

  const std::vector<SSAPtr> &args() { return _args; }
};

class JumpInst : public TerminatorInst {
public:
  explicit JumpInst(const SSAPtr &target, const SSAPtr & IB = nullptr)
    : TerminatorInst(Instruction::TermOps::Jmp,1, IB) {
    AddValue(target);
  }

  unsigned GetSuccessorNum() const override { return 1; }

  SSAPtr GetSuccessor(unsigned idx) const override {
    DBG_ASSERT(idx == 0, "index out of range");
    return target();
  };

  void SetSuccessor(unsigned idx, const BlockPtr &B) override {
    DBG_ASSERT(idx == 0, "index out of range");
    this->SetOperand(0, B);
  }

  const SSAPtr &target() const { return (*this)[0].get(); }
};

// store to alloc
// operands: value, pointer
class StoreInst : public Instruction {
public:
  StoreInst(const SSAPtr &V, const SSAPtr &P, const SSAPtr &IB = nullptr)
  : Instruction(Instruction::MemoryOps::Store, 2, IB) {
    AddValue(V);
    AddValue(P);
  }

  // getters
  const SSAPtr &value() const { return (*this)[0].get(); }

  const SSAPtr &pointer() const { return (*this)[1].get(); }
};

// alloc on stack
class AllocaInst : public User {
public:
  AllocaInst() = default;
};

// load from pointer
class LoadInst : public Instruction {
public:
  LoadInst(const SSAPtr &ptr, const SSAPtr &IB = nullptr)
    : Instruction(Instruction::MemoryOps::Load, 1, IB) {
    AddValue(ptr);
  }

  // getter/setter
  void SetPointer(const SSAPtr &ptr)    { (*this)[0].set(ptr); }
  const SSAPtr &Pointer()         const { return (*this)[0].get(); }
};


// argument reference
class ArgRefSSA : public Value {
private:
  SSAPtr      _func;
  std::size_t _index;

public:
  ArgRefSSA(const SSAPtr &func, std::size_t index)
    : _func(func), _index(index) {}

  // getter
  const SSAPtr &func()  const { return _func;  }
  std::size_t   index() const { return _index; }
};

// return from function
// operand: value
class ReturnInst : public User {
public:
  explicit ReturnInst(const SSAPtr &value) { AddValue(value); }

  // getter/setter
  const SSAPtr &RetVal()    const { return (*this)[0].get(); }
  void SetRetVal(const SSAPtr &value) { (*this)[0].set(value); }
};

}
#endif //RJIT_SSA_H
