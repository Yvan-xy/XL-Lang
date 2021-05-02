#include "use.h"
#include "value.h"

namespace RJIT::mid {
  void Use::removeFromList(const SSAPtr& V) {
    V->removeUse(this);
  }

  void Use::addToList(const SSAPtr& V) {
    V->addUse(this);
  }
}
