#pragma once

#include "mc/deps/application/AppPlatform.h"

class AppPlatformEvent {
  public:
    AppPlatform *mAppPlatform;

    explicit AppPlatformEvent(AppPlatform *appPlatform);
};

class AppPlatformInitializeEvent : public AppPlatformEvent {
  public:
    explicit AppPlatformInitializeEvent(AppPlatform *appPlatform);
};
