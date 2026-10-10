#pragma once

#include "mc/resources/ResourcePackManager.h"

class ResourcePackManagerEvent {
  public:
    ResourcePackManager *mResourcePackManager;

    explicit ResourcePackManagerEvent(ResourcePackManager *resourcePackManager);
};

class ResourcePackManagerBeforeComposeFullStackEvent : public ResourcePackManagerEvent {
  public:
    explicit ResourcePackManagerBeforeComposeFullStackEvent(ResourcePackManager *resourcePackManager);
};

class ResourcePackManagerAfterComposeFullStackEvent : public ResourcePackManagerEvent {
  public:
    explicit ResourcePackManagerAfterComposeFullStackEvent(ResourcePackManager *resourcePackManager);
};
