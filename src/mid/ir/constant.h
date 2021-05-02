#ifndef RJIT_CONSTANT_H
#define RJIT_CONSTANT_H

#include "user.h"
#include "define/type.h"

namespace RJIT::mid {

// Constant int value ssa
class ConstantInt : public Value {
private:
  unsigned int _value;

public:
  explicit ConstantInt(unsigned int value) : _value(value) {}

  unsigned int value() const { return _value; }

  bool IsZero() const { return _value == 0; }
};

// Constant string value ssa
class ConstantString : public Value {
private:
  std::string _str;

public:
  ConstantString(const std::string &str) : _str(str) {}

  const std::string &value() const { return _str; }
};

SSAPtr GetZeroValue(TYPE::Type type) {
  using TYPE::Type;
  auto zero = std::make_shared<ConstantInt>(0);
  switch (type) {
    case Type::Void:
      zero->set_type(TYPE::MakeVoid());
      break;
    case Type::Int8:
      zero->set_type(TYPE::MakePrimType(Type::Int8, true));
      break;
    case Type::UInt8:
      zero->set_type(TYPE::MakePrimType(Type::UInt8, true));
      break;
    case Type::Int32:
      zero->set_type(TYPE::MakePrimType(Type::Int32, true));
      break;
    case Type::UInt32:
      zero->set_type(TYPE::MakePrimType(Type::UInt32, true));
      break;
    default:
      DBG_ASSERT(0, "Get error zero type");
      break;
  }
  return zero;
}

}

#endif //RJIT_CONSTANT_H
