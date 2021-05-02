#ifndef RJIT_TYPE_H
#define RJIT_TYPE_H

#include <memory>
#include <utility>
#include <vector>
#include <utility>
#include <optional>

#include "lib/debug.h"

namespace RJIT::TYPE {
enum class Type {
  Void, Bool,
  UInt8, Int8,
  UInt32, Int32,
  String, Dam
};

Type string2Type(const std::string &type);

std::string type2String(Type type);

class TypeInfo;

using TypeInfoPtr = std::shared_ptr<TypeInfo>;
using TypePtrList = std::vector<TypeInfoPtr>;

class TypeInfo {
public:
  virtual ~TypeInfo() = default;

  virtual Type GetType() { return Type::Dam; }

  virtual std::size_t GetSize() const = 0;

  // return true if is right value
  virtual bool IsRightValue() const = 0;

  // return true if is void type
  virtual bool IsVoid() const = 0;

  // return true if is integer
  virtual bool IsInteger() const = 0;

  // return true if is unsigned
  virtual bool IsUnsigned() const = 0;

  // return true if is constant type
  virtual bool IsConst() const = 0;

  // return true if is function type
  virtual bool IsFunction() const = 0;

  // return true if is prime type
  virtual bool IsPrime() const = 0;

  virtual bool IsBool() const = 0;

  // return the type of arguments of a function call
  virtual std::optional<TypePtrList> GetArgsType() const = 0;

  // return the return type of a function call
  virtual TypeInfoPtr GetReturnType() const = 0;

  // return the identifier of current type
  virtual std::string GetTypeId() const = 0;

  // return a new type with specific value type (left/right)
  virtual TypeInfoPtr GetValueType(bool is_right) const = 0;

  virtual bool operator==(const TypeInfoPtr &type) {
    DBG_ASSERT(0, "can't compare with Base type");
    return false;
  }

  virtual bool IsLonger(const TypeInfoPtr &type) {
    DBG_ASSERT(0, "can't compare with Base type");
    return false;
  }

  virtual bool IsNotShortThan(const TypeInfoPtr &type) {
    DBG_ASSERT(0, "can't compare with Base type");
    return false;
  }
};

class PrimType : public TypeInfo {
private:
  Type type_;
  bool is_right_;

public:
  PrimType(Type tp, bool ir) : type_(tp), is_right_(ir) {}

  ~PrimType() override = default;

  Type GetType() override { return type_; }

  std::size_t GetSize() const override;

  bool IsRightValue() const override { return is_right_; }

  bool IsVoid() const override { return type_ == Type::Void; }

  bool IsInteger() const override {
    auto t = static_cast<int>(type_);
    return t >= static_cast<int>(Type::UInt8) &&
           t <= static_cast<int>(Type::Int32);
  }

  bool IsUnsigned() const override {
    auto t = static_cast<int>(type_);
    return t == static_cast<int>(Type::UInt8) ||
           t == static_cast<int>(Type::UInt32);
  }

  bool IsPrime() const override { return true; }

  bool IsBool() const override { return type_ == Type::Bool; }

  bool IsConst() const override { return false; }

  bool IsFunction() const override { return false; }

  std::optional<TypePtrList> GetArgsType() const override { return {}; }

  TypeInfoPtr GetReturnType() const override {
    return nullptr;
  }

  std::string GetTypeId() const override;

  Type type() const { return type_; }

  TypeInfoPtr GetValueType(bool is_right) const override;

  bool operator==(const TypeInfoPtr &typeInfo) override {
    return type_ == typeInfo->GetType();
  }

  bool IsLonger(const TypeInfoPtr &type) override {
    DBG_ASSERT(type->IsPrime(), "can't compare with non-prime type");
    return type_ > type->GetType();
  }

  bool IsNotShortThan(const TypeInfoPtr &type) override {
    DBG_ASSERT(type->IsPrime(), "can't compare with non-prime type");
    return type_ >= type->GetType();
  }
};

class ConstType : public TypeInfo {
private:
  TypeInfoPtr type;
public:
  explicit ConstType(TypeInfoPtr type_) : type(std::move(type_)) {}

  std::size_t GetSize() const override { return type->GetSize(); }

  bool IsRightValue() const override { return type->IsRightValue(); }

  bool IsVoid() const override { return type->IsVoid(); }

  bool IsInteger() const override { return type->IsInteger(); }

  bool IsUnsigned() const override { return type->IsUnsigned(); }

  bool IsConst() const override { return true; }

  bool IsFunction() const override { return type->IsFunction(); }

  bool IsPrime() const override { return type->IsPrime(); }

  bool IsBool() const override { return type->IsBool(); }

  std::optional<TypePtrList> GetArgsType() const override { return {}; }

  TypeInfoPtr GetReturnType() const override {
    return nullptr;
  }

  std::string GetTypeId() const override;

  TypeInfoPtr GetValueType(bool is_right) const override;

  Type GetType() override { return type->GetType(); }
};

class FuncType : public TypeInfo {
private:
  TypePtrList args_;
  TypeInfoPtr ret_;
  bool is_right_;
public:
  FuncType(TypePtrList args, TypeInfoPtr ret, bool is_right)
      : args_(std::move(args)), ret_(std::move(ret)),
        is_right_(is_right) {}

  bool IsRightValue() const override { return is_right_; }

  bool IsVoid() const override { return false; }

  bool IsInteger() const override { return false; }

  bool IsUnsigned() const override { return false; }

  bool IsConst() const override { return false; }

  bool IsFunction() const override { return true; }

  bool IsPrime() const override { return false; }

  bool IsBool() const override { return false; }

  std::optional<TypePtrList> GetArgsType() const override { return args_; }

  TypeInfoPtr GetReturnType() const override;

  std::string GetTypeId() const override;

  TypeInfoPtr GetValueType(bool is_right) const override;

  std::size_t GetSize() const override { return 4; }

};

// create a new primitive type
inline TypeInfoPtr MakePrimType(Type type, bool is_right) {
  return std::make_shared<PrimType>(type, is_right);
}

// create a new void type
inline TypeInfoPtr MakeVoid() {
  return std::make_shared<PrimType>(Type::Void, true);
}
}

#endif //RJIT_TYPE_H
