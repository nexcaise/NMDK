#pragma once

#include "mc/_HeaderOutputPredefine.h"

#include "mc/world/actor/ActorFlags.h"

// clang-format off
class EntityContext;
// clang-format on

namespace SynchedActorDataAccess {

bool getActorFlag(::EntityContext const &entity, ::ActorFlags flag);

void setActorFlag(::EntityContext &entity, ::ActorFlags flag, bool value);

} // namespace SynchedActorDataAccess
