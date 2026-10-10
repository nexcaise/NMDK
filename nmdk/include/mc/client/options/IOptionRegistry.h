#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/options/GraphicsMode.h"

class IOptionRegistry {
  public:
    GraphicsMode getGraphicsMode() const;
    void saveIfNeeded();
};
