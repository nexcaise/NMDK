#pragma once

#include "mc/_HeaderOutputPredefine.h"

#include "mc/world/actor/ActorFlags.h"

class EntityContext;

namespace SynchedActorDataAccess {

bool getActorFlag(::EntityContext const &entity, ::ActorFlags flag);

void setActorFlag(::EntityContext &entity, ::ActorFlags flag, bool value);

} // namespace SynchedActorDataAccess
