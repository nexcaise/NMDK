#pragma once

#include "mc/_HeaderOutputPredefine.h"

class AbstractScene;

class ISceneStack {
  public:
    AbstractScene *getTopScene();
};
