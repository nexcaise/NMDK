#pragma once

#include "mc/_HeaderOutputPredefine.h"

class ResourceLocation;

class ResourcePackManager {
  public:
    bool load(::ResourceLocation const &resourceLocation, ::std::string &resourceStream) const;

    void _composeFullStack();
};
