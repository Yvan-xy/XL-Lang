#ifndef RJIT_TYPE_H
#define RJIT_TYPE_H

namespace RJIT::TYPE {
    enum class Type {
        Void,
        Int8, Int32,
        UInt8, UInt32, String, Dam
    };

     Type string2Type(const std::string &type);

     std::string type2String(Type type);
}

#endif //RJIT_TYPE_H
