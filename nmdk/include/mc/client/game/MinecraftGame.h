#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/resources/ResourcePackManager.h"

class MinecraftGame {
  public:
    void _finishReloadingResources(::ResourcePackManager &mgr, bool fullVanillaPackOnStack);
};
