#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/client/social/ToastMessage.h"

class ToastManager {
  public:
    void pushToast(::ToastMessage *message);
};
