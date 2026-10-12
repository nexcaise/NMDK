#pragma once

#include "mc/_HeaderOutputPredefine.h"

#if NMDK_HAS_ENTT

#include "mc/world/level/GameType.h"

struct ActorGameTypeComponent {
  public:
    ::GameType mGameType;
};

#endif
