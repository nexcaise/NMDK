#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/deps/input/enums/ButtonState.h"

class TouchGlyphButtonControl {
  public:
    CLASS_FIELD(uint, mId, 0xC0);
    CLASS_FIELD(ButtonState, mState, 0xC4);
};
