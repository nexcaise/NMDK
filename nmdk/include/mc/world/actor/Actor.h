#pragma once

#include "mc/_HeaderOutputPredefine.h"

#include "mc/deps/ecs/EntityContext.h"
#include "mc/entity/components/ActorEquipmentComponent.h"
#include "mc/entity/components/ActorGameTypeComponent.h"
#include "mc/world/actor/ActorSwingSource.h"
#include "mc/world/actor/provider/SynchedActorDataAccess.h"
#include "mc/world/item/HandSlot.h"

class Actor {
  public:
#if NMDK_HAS_ENTT
    CLASS_FIELD(EntityContext, mEntityContext, 0x8);

    template <typename T> T *tryGetComponent() { return mEntityContext.enttRegistry.try_get<T>(mEntityContext.entity); }

    ItemStackBase *getArmor(int slot) {
        auto *component = tryGetComponent<ActorEquipmentComponent>();
        return component && component->armorContainer ? component->armorContainer->getItem(slot) : nullptr;
    }

    ItemStackBase *getHand(int slot) {
        auto *component = tryGetComponent<ActorEquipmentComponent>();
        return component && component->handContainer ? component->handContainer->getItem(slot) : nullptr;
    }

    GameType getGameMode() {
        auto *component = tryGetComponent<ActorGameTypeComponent>();
        return component ? component->mGameType : GameType::Undefined;
    }

    bool getStatusFlag(::ActorFlags flag) const { return ::SynchedActorDataAccess::getActorFlag(mEntityContext, flag); }

    void setStatusFlag(::ActorFlags flag, bool value) {
        ::SynchedActorDataAccess::setActorFlag(mEntityContext, flag, value);
    }
#else
    template <typename T> T *tryGetComponent() { return nullptr; }

    ::ItemStackBase *getArmor(int slot) { return nullptr; }

    ::ItemStackBase *getHand(int slot) { return nullptr; }

    ::GameType getGameMode() { return ::GameType::Undefined; }

    bool getStatusFlag(::ActorFlags flag) const { return false; }

    void setStatusFlag(::ActorFlags flag, bool value) {}
#endif

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
