#pragma once

#include "mc/_HeaderOutputPredefine.h"

#if NMDK_HAS_ENTT

struct EntityId {
    uint rawId;

    [[nodiscard]] constexpr bool operator==(const EntityId &other) const = default;

    [[nodiscard]] constexpr operator uint() const { return this->rawId; }
};

#else

struct EntityId {};

#endif
