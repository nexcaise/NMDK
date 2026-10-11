#pragma once

#include "nmdk/api/Types.h"

class InputEvent {
  public:
    InputEvent() = default;
};

class CancelableInputEvent : public InputEvent {
  public:
    CancelableInputEvent() = default;
};

class TouchButtonGlyphEvent : public InputEvent {
  public:
    bool mIsDown;
    uint mButtonId;

    TouchButtonGlyphEvent(bool isDown, uint buttonId) : mIsDown(isDown), mButtonId(buttonId) {}
};
