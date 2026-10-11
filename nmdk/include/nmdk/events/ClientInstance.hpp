#pragma once

#include "mc/client/game/ClientInstance.h"

// NOTE: event classes are header-only on purpose. Having a second, different
// definition in another header (ODR violation) made the hook write past the
// end of the stack object and crash.

class ClientInstanceEvent {
  public:
    ClientInstance *mClient;

    explicit ClientInstanceEvent(ClientInstance *client) : mClient(client) {}
};

class ClientInstanceInitFinishedEvent : public ClientInstanceEvent {
  public:
    explicit ClientInstanceInitFinishedEvent(ClientInstance *client) : ClientInstanceEvent(client) {}
};

class ClientInstanceIsPreGameEvent : public ClientInstanceEvent {
  private:
    bool mIsPreGame;

  public:
    ClientInstanceIsPreGameEvent(ClientInstance *client, bool isPreGame)
        : ClientInstanceEvent(client), mIsPreGame(isPreGame) {}

    bool getIsPreGame() const { return mIsPreGame; }
    void setIsPreGame(bool isPreGame) { mIsPreGame = isPreGame; }
};
