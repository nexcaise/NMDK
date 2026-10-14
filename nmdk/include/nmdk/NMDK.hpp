#pragma once

#include <string>

#include "nmdk/EventBus.hpp"
#include "nmdk/api/Macro.h"

#include "nmdk/Compatibility.hpp"
#include "nmdk/HookRegistry.hpp"
#include "nmdk/Library.hpp"
#include "nmdk/MemoryScanner.hpp"
#include "nmdk/Patch.hpp"
#include "nmdk/Types.hpp"
#include "nmdk/VTable.hpp"

namespace nmdk {

std::string getVersion();

EventBus &getEventBus();

} // namespace nmdk
