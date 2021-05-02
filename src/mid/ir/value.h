#ifndef RJIT_VALUE_H
#define RJIT_VALUE_H

#include "use.h"
#include "define/type.h"
#include "front/logger.h"

namespace RJIT::mid {

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

class Value {
private:
  UseList           _use_list;
  front::LoggerPtr  _logger;
  TYPE::TypeInfoPtr _type;

public:
  void addUse(Use *U) { _use_list.emplace_back(U); }

  void removeUse(Use *U) { _use_list.remove(U); }

  void set_logger(const front::LoggerPtr &logger) { _logger = logger;}

  void set_type(const TYPE::TypeInfoPtr &type) { _type = type; }

  virtual BlockPtr GetParent() { return nullptr; }

  const UseList &uses() const { return _use_list; }
  const front::LoggerPtr &logger() const { return _logger; }
  const TYPE::TypeInfoPtr &type() const { return _type; }
};
};

#endif //RJIT_VALUE_H
