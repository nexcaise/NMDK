#pragma once

#include "mc/_HeaderOutputPredefine.h"

#include "mc/deps/ecs/EntityContext.h"
#include "mc/entity/components/ActorEquipmentComponent.h"
#include "mc/entity/components/ActorGameTypeComponent.h"
#include "mc/world/actor/ActorSwingSource.h"
#include "mc/world/item/HandSlot.h"

class Actor {
  public:
    CLASS_FIELD(EntityContext, entityContext, 0x8);

  public:
    template <typename T> T *tryGetComponent() { return entityContext.enttRegistry.try_get<T>(entityContext.entity); }

    ItemStackBase *getArmor(int slot) {
        return this->tryGetComponent<ActorEquipmentComponent>()->armorContainer->getItem(slot);
    }

    ItemStackBase *getHand(int slot) {
        return this->tryGetComponent<ActorEquipmentComponent>()->handContainer->getItem(slot);
    }

    GameType getGameMode() { return this->tryGetComponent<ActorGameTypeComponent>()->mGameType; }

  public:
    bool swing(ActorSwingSource actorSwingSource, HandSlot handSlot);
};
