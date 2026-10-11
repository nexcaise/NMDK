#pragma once

#include "mc/deps/input/enums/TouchButtonGlyphType.h"

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
    TouchButtonGlyphType mButtonType;

    TouchButtonGlyphEvent(bool isDown, TouchButtonGlyphType buttonType) : mIsDown(isDown), mButtonType(buttonType) {}
};
