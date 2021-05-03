#include "mid/ir/usedef/use.h"
#include "mid/ir/usedef/value.h"

namespace RJIT::mid {
  void Use::removeFromList(const SSAPtr& V) {
    V->removeUse(this);
  }

  void Use::addToList(const SSAPtr& V) {
    V->addUse(this);
  }
}
