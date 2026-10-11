#pragma once

#include <string>

#include "nmdk/EventBus.hpp"
#include "nmdk/api/Macro.h"

// gamepwnage-backed primitives: hooks, memory, signatures, patches, vtables.
#include "nmdk/Compatibility.hpp"
#include "nmdk/Hook.hpp"
#include "nmdk/Library.hpp"
#include "nmdk/Memory.hpp"
#include "nmdk/Patch.hpp"
#include "nmdk/Process.hpp"
#include "nmdk/Signature.hpp"
#include "nmdk/Types.hpp"
#include "nmdk/VTable.hpp"

namespace nmdk {

std::string getVersion();

EventBus &getEventBus();

} // namespace nmdk
