#pragma once

#if NMDK_HAS_ENTT

#include "mc/deps/ecs/EntityId.h"

struct EntityIdTraits {
    using value_type = EntityId;

    using entity_type = uint;
    using version_type = ushort;

    static constexpr entity_type entity_mask = 0x3FFFF;
    static constexpr entity_type version_mask = 0x3FFF;
};

#else

struct EntityIdTraits {};

#endif
