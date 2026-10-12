#pragma once

#include "mc/_HeaderOutputPredefine.h"

#if NMDK_HAS_ENTT

#include "mc/deps/ecs/IEntityComponent.h"
#include "mc/world/Container.h"
#include "mc/world/item/ItemStackBase.h"

struct ActorEquipmentComponent : IEntityComponent {
    static constexpr uint type_hash = 0xB06141A9;

    Container *handContainer;
    Container *armorContainer;
};

#endif
