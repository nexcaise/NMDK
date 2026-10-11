#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/world/item/ItemStackBase.h"

class Container {
  public:
    ItemStackBase *getItem(int);
};
