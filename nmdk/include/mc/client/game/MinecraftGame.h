#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/resources/ResourcePackManager.h"

class MinecraftGame {
  public:
    void *_onResumeWaitReloadTextures();

    void onActiveResourcePacksChanged(ResourcePackManager &);

    void _loadInitialPackResources(ResourcePackManager &, bool);

    void _finishReloadingResources(ResourcePackManager &, bool);

    void _loadGlobalResourcePacks();

    void cleanReloadMaterials();

    void _beginReloadingResources();
};
