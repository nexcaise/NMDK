#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/client/gui/ScreenSizeData.h"

struct GuiData {
  public:
    std::byte pad0[0x40];
    ::ScreenSizeData mScreenSizeData;
    bool mScreenSizeDataValid;
    float mGuiScale;
    float mInvGuiScale;
};
