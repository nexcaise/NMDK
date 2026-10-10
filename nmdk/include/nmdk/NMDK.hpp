#pragma once

#include <string>

#include "nmdk/EventBus.hpp"
#include "nmdk/api/Macro.h"

namespace nmdk {

std::string getVersion();

EventBus &getEventBus();

} // namespace nmdk
