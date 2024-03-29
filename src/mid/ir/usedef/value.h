#ifndef RJIT_VALUE_H
#define RJIT_VALUE_H

#include <ostream>

#include "define/type.h"
#include "front/logger.h"
#include "mid/ir/usedef/use.h"

namespace RJIT::mid {

class IdManager;

class Constant;
class Argument;
class Instruction;
class BasicBlock;
class GlobalValue;
class Function;
class BinaryOperator;
class GlobalVariable;
class GlobalAlias;
class ValueSymbolTable;
class TypeSymbolTable;


// type aliases
using FuncPtr       = std::shared_ptr<Function>;
using InstPtr       = std::shared_ptr<Instruction>;
using BlockPtr      = std::shared_ptr<BasicBlock>;
using BinaryPtr     = std::shared_ptr<BinaryOperator>;
using GlobalVarPtr  = std::shared_ptr<GlobalVariable>;
using Blocks        = std::vector<BlockPtr>;

class Value {
private:
  TYPE::TypeInfoPtr _type;
  front::LoggerPtr  _logger;
  BlockPtr          _parent;  // block
  UseList           _use_list;

public:
  Value() = default;
  virtual ~Value() = default;

  void addUse(Use *U) { _use_list.emplace_back(U); }

  void removeUse(Use *U) { _use_list.remove(U); }

  void set_logger(const front::LoggerPtr &logger) { _logger = logger;}

  void set_type(const TYPE::TypeInfoPtr &type) { _type = type; }

  virtual const BlockPtr &GetParent() const { return _parent; }
  virtual void SetParent(const BlockPtr &BB) { _parent = BB; }

  void ReplaceBy(const SSAPtr &value) {
    if (value.get() == this) return;
    // reroute all uses to new value
    while (!_use_list.empty()) {
      _use_list.front()->set(value);
    }
  }

  void RemoveFromUser();

  // dump the content of SSA value to output stream
  virtual void Dump(std::ostream &os, IdManager &id_mgr) const = 0;

  virtual bool isInstruction() const { return false; }
  const UseList &uses() const { return _use_list; }
  const front::LoggerPtr &logger() const { return _logger; }
  const TYPE::TypeInfoPtr &type() const { return _type; }
};
};

#endif //RJIT_VALUE_H
