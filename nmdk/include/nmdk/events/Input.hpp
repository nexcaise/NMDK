#pragma once

#include "mc/deps/input/enums/MinecraftInputType.h"

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
    MinecraftInputType mInputType;

    TouchButtonGlyphEvent(bool isDown, MinecraftInputType inputType) : mIsDown(isDown), mInputType(inputType) {}
};

class TapEvent : public InputEvent {
  public:
    bool mIsDown;
    MinecraftInputType mInputType;

    TapEvent(bool isDown, MinecraftInputType inputType) : mIsDown(isDown), mInputType(inputType) {}
};
