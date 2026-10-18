#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/client/social/ToastMessageType.h"

class ToastMessage {
  public:
    CLASS_FIELD(::ToastMessageType, mType, 0x0C);
};
