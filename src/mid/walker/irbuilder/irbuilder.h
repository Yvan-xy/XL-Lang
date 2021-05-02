#ifndef RJIT_IRBUILDER_H
#define RJIT_IRBUILDER_H

#include "mid/ir/value.h"
#include "mid/visitor/visitor.h"

namespace RJIT::mid {

class IRBuilder : Visitor<SSAPtr> {

};

}

#endif //RJIT_IRBUILDER_H
