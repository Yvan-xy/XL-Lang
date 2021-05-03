#ifndef RJIT_CONSTANT_H
#define RJIT_CONSTANT_H

#include "mid/ir/usedef/user.h"
#include "define/type.h"

namespace RJIT::mid {

// Constant int value ssa
class ConstantInt : public Value {
private:
  unsigned int _value;

public:
  explicit ConstantInt(unsigned int value) : _value(value) {}

  // dump ir
  void Dump(std::ostream &os, IdManager &id_mgr) const override;

  unsigned int value() const { return _value; }

  bool IsZero() const { return _value == 0; }
};

// Constant string value ssa
class ConstantString : public Value {
private:
  std::string _str;

public:
  ConstantString(const std::string &str) : _str(str) {}

  // dump ir
  void Dump(std::ostream &os, IdManager &id_mgr) const override;

  const std::string &value() const { return _str; }
};

SSAPtr GetZeroValue(TYPE::Type type);

SSAPtr GetAllOneValue(TYPE::Type type);

}

#endif //RJIT_CONSTANT_H
