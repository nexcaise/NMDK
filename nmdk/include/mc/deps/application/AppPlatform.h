#pragma once

#include "mc/_HeaderOutputPredefine.h"

class AppPlatform {
  public:
    void _fireAppSuspended();
    void _fireAppResumed();
};
