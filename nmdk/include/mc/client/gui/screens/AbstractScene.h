#pragma once

#include "mc/_HeaderOutputPredefine.h"

class ScreenSizeData;

class AbstractScene {
  public:
    void setSize(::ScreenSizeData const &screenSizeData);
    std::string getScreenName() const;
};
