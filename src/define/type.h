#ifndef RJIT_TYPE_H
#define RJIT_TYPE_H

#include <string>
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

  // return true if is bool type
  virtual bool IsBool() const = 0;

  virtual bool IsPointer() const = 0;

  // return the type of arguments of a function call
  virtual std::optional<TypePtrList> GetArgsType() const = 0;

  // return the return type of a function call
  virtual TypeInfoPtr GetReturnType() const = 0;

  // return the identifier of current type
  virtual std::string GetTypeId() const = 0;

  // return a new type with specific value type (left/right)
  virtual TypeInfoPtr GetValueType(bool is_right) const = 0;

  virtual TypeInfoPtr GetDereferenceType() const = 0;

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
  Type _type;
  bool is_right_;

public:
  PrimType(Type tp, bool ir) : _type(tp), is_right_(ir) {}

  ~PrimType() override = default;

  Type GetType() override { return _type; }

  std::size_t GetSize() const override;

  bool IsRightValue() const override { return is_right_; }

  bool IsVoid() const override { return _type == Type::Void; }

  bool IsInteger() const override {
    auto t = static_cast<int>(_type);
    return (t >= static_cast<int>(Type::UInt8) &&
           t <= static_cast<int>(Type::Int32)) || t == static_cast<int>(Type::Bool);
  }

  bool IsUnsigned() const override {
    auto t = static_cast<int>(_type);
    return t == static_cast<int>(Type::UInt8) ||
           t == static_cast<int>(Type::UInt32);
  }

  bool IsPrime() const override { return true; }

  bool IsBool() const override { return _type == Type::Bool; }

  bool IsConst() const override { return false; }

  bool IsFunction() const override { return false; }

  bool IsPointer() const override { return false;}

  std::optional<TypePtrList> GetArgsType() const override { return {}; }

  TypeInfoPtr GetReturnType() const override {
    return nullptr;
  }

  std::string GetTypeId() const override;

  Type type() const { return _type; }

  TypeInfoPtr GetValueType(bool is_right) const override;

  TypeInfoPtr GetDereferenceType() const override {
    return nullptr;
  }

  bool operator==(const TypeInfoPtr &typeInfo) override {
    return _type == typeInfo->GetType();
  }

  bool IsLonger(const TypeInfoPtr &type) override {
    DBG_ASSERT(type->IsPrime(), "can't compare with non-prime type");
    return _type > type->GetType();
  }

  bool IsNotShortThan(const TypeInfoPtr &type) override {
    DBG_ASSERT(type->IsPrime(), "can't compare with non-prime type");
    return _type >= type->GetType();
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

  bool IsPointer() const override { return type->IsPointer(); }


  std::optional<TypePtrList> GetArgsType() const override { return {}; }

  TypeInfoPtr GetReturnType() const override {
    return nullptr;
  }

  std::string GetTypeId() const override;

  TypeInfoPtr GetValueType(bool is_right) const override;

  Type GetType() override { return type->GetType(); }

  TypeInfoPtr GetDereferenceType() const override {
    return type->GetDereferenceType();
  }

  bool operator==(const TypeInfoPtr &typeInfo) override {
    return type->GetType() == typeInfo->GetType();
  }

  bool IsNotShortThan(const TypeInfoPtr &typeInfo) override {
    DBG_ASSERT(typeInfo->IsPrime(), "can't compare with non-prime type");
    return type->GetType() >= typeInfo->GetType();
  }
};

class FuncType : public TypeInfo {
private:
  TypePtrList _args;
  TypeInfoPtr _ret;
  bool _is_right;
public:
  FuncType(TypePtrList args, TypeInfoPtr ret, bool is_right)
      : _args(std::move(args)), _ret(std::move(ret)),
        _is_right(is_right) {}

  bool IsRightValue() const override { return _is_right; }
  bool IsVoid()       const override { return false;     }
  bool IsInteger()    const override { return false;     }
  bool IsUnsigned()   const override { return false;     }
  bool IsConst()      const override { return false;     }
  bool IsFunction()   const override { return true;      }
  bool IsPrime()      const override { return false;     }
  bool IsBool()       const override { return false;     }
  bool IsPointer()    const override { return false;     }


  std::optional<TypePtrList> GetArgsType() const override { return _args; }

  TypeInfoPtr GetReturnType() const override;

  std::string GetTypeId() const override;

  TypeInfoPtr GetValueType(bool is_right) const override;

  std::size_t GetSize() const override { return 4; }

  TypeInfoPtr GetDereferenceType() const override {
    return nullptr;
  }

  bool operator==(const TypeInfoPtr &typeInfo) override;
};


class PointerType : public TypeInfo {
private:
  TypeInfoPtr _base;
  bool _is_right;
public:
  PointerType(TypeInfoPtr base, bool is_right)
      : _base(std::move(base)), _is_right(is_right) {}

  bool IsRightValue() const override { return _is_right; }
  bool IsVoid()       const override { return false;     }
  bool IsInteger()    const override { return false;     }
  bool IsUnsigned()   const override { return false;     }
  bool IsConst()      const override { return false;     }
  bool IsFunction()   const override { return true;      }
  bool IsPrime()      const override { return false;     }
  bool IsBool()       const override { return false;     }
  bool IsPointer()    const override { return true;      }


  std::optional<TypePtrList> GetArgsType() const override { return {}; }

  TypeInfoPtr GetReturnType() const override { return nullptr; };

  std::string GetTypeId() const override;

  TypeInfoPtr GetValueType(bool is_right) const override;

  std::size_t GetSize() const override { return 4; }

  TypeInfoPtr GetDereferenceType() const override { return _base; }

  bool operator==(const TypeInfoPtr &typeInfo) override {
    DBG_ASSERT(typeInfo->IsPointer(), "compare with non-pointer type");
    return _base == typeInfo->GetDereferenceType();
  }
};

// create a new primitive type
inline TypeInfoPtr MakePrimType(Type type, bool is_right) {
  return std::make_shared<PrimType>(type, is_right);
}

// create a new primitive type
inline TypeInfoPtr MakeConst(Type type, bool is_right = true) {
  return std::make_shared<ConstType>(MakePrimType(type, is_right));
}

// create a new void type
inline TypeInfoPtr MakeVoid() {
  return std::make_shared<PrimType>(Type::Void, true);
}

inline TypeInfoPtr
MakeFuncType(TypePtrList args, TypeInfoPtr ret, bool is_right) {
  return std::make_shared<FuncType>(args, ret, is_right);
}

inline TypeInfoPtr
MakePointerType(const TypeInfoPtr &type) {
  return std::make_shared<PointerType>(type, true);
}

}

#endif //RJIT_TYPE_H
