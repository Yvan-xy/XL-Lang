#ifndef RJIT_TYPE_H
#define RJIT_TYPE_H

#include <memory>

namespace RJIT::TYPE {
    enum class Type {
        Void,
        Int8, Int32,
        UInt8, UInt32, String, Const, Dam
    };

    class TypeInfo {
    private:
        Type type_;
    public:
        TypeInfo() : type_(Type::Void) {}

        explicit TypeInfo(Type type) : type_(type) {}

        Type type() { return type_; }

        bool operator==(TypeInfo *p) {
            return p->type() == this->type_;
        }

        bool IsConst() const { return type_ == Type::Const; }
    };

    typedef std::shared_ptr<TypeInfo> TypeInfoPtr;

    Type string2Type(const std::string &type);

    std::string type2String(Type type);
}

#endif //RJIT_TYPE_H
