#pragma once

#include "mc/client/game/ClientInstance.h"

class ClientInstanceEvent {
  public:
    ClientInstance *mClient;

    explicit ClientInstanceEvent(ClientInstance *client);
};

class ClientInstanceInitFinishedEvent : public ClientInstanceEvent {
  public:
    ClientInstanceInitFinishedEvent(ClientInstance *client);
};