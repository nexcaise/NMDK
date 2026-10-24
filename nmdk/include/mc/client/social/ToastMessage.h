#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/client/social/ToastMessageType.h"

class ToastMessage {
  public:
    ::std::byte padding[0xC];
    ::ToastMessageType mType;
    ::std::string mTitle;
    ::std::string mSubtitle;
};
