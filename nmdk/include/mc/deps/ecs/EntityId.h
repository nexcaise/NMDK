#pragma once

#include "mc/_HeaderOutputPredefine.h"

struct EntityId {
    uint rawId;

    [[nodiscard]] constexpr bool operator==(const EntityId &other) const = default;

    [[nodiscard]] constexpr operator uint() const { return this->rawId; }
};
