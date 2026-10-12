#pragma once

#include "mc/_HeaderOutputPredefine.h"

#include "mc/deps/ecs/EntityContext.h"
#include "mc/entity/components/ActorEquipmentComponent.h"
#include "mc/entity/components/ActorGameTypeComponent.h"
#include "mc/world/actor/ActorSwingSource.h"
#include "mc/world/actor/provider/SynchedActorDataAccess.h"
#include "mc/world/item/HandSlot.h"

class Actor {
#if NMDK_HAS_ENTT
  public:
    CLASS_FIELD(EntityContext, mEntityContext, 0x8);

  public:
    template <typename T> T *tryGetComponent() { return mEntityContext.enttRegistry.try_get<T>(mEntityContext.entity); }

    ItemStackBase *getArmor(int slot) {
        return this->tryGetComponent<ActorEquipmentComponent>()->armorContainer->getItem(slot);
    }

    ItemStackBase *getHand(int slot) {
        return this->tryGetComponent<ActorEquipmentComponent>()->handContainer->getItem(slot);
    }

    GameType getGameMode() { return this->tryGetComponent<ActorGameTypeComponent>()->mGameType; }

#else
  public:
    template <typename T> T *tryGetComponent() { return nullptr; }

    ::ItemStackBase *getArmor(int slot) { return nullptr; }

    ::ItemStackBase *getHand(int slot) { return nullptr; }

    ::GameType getGameMode() { return ::GameType::Undefined; }

#endif

    bool getStatusFlag(::ActorFlags flag) const { return ::SynchedActorDataAccess::getActorFlag(mEntityContext, flag); }

    void setStatusFlag(::ActorFlags flag, bool value) {
        ::SynchedActorDataAccess::setActorFlag(mEntityContext, flag, value);
    }

    [[nodiscard]] bool isSwimming() const { return getStatusFlag(::ActorFlags::Swimming); }
    [[nodiscard]] bool isMoving() const { return getStatusFlag(::ActorFlags::Moving); }
    [[nodiscard]] bool isSprinting() const { return getStatusFlag(::ActorFlags::Sprinting); }
    [[nodiscard]] bool isSneaking() const { return getStatusFlag(::ActorFlags::Sneaking); }
    [[nodiscard]] bool isCrawling() const { return getStatusFlag(::ActorFlags::Crawling); }
    [[nodiscard]] bool isGliding() const { return getStatusFlag(::ActorFlags::Gliding); }
    [[nodiscard]] bool isDancing() const { return getStatusFlag(::ActorFlags::Dancing); }

  public:
    bool swing(::ActorSwingSource actorSwingSource, ::HandSlot handSlot);
};
