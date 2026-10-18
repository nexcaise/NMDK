#pragma once

#include "mc/client/social/ToastManager.h"
#include "mc/client/social/ToastMessage.h"
#include "mc/client/social/ToastMessageType.h"

class ToastManagerEvent {
  public:
    ToastManager *mToastManager;

    explicit ToastManagerEvent(ToastManager *toastManager) : mToastManager(toastManager) {}
};

class ToastManagerPushToastEvent : public ToastManagerEvent {
  private:
    ToastMessage *mMessage;
    bool mCancelled{false};

  public:
    ToastManagerPushToastEvent(ToastManager *toastManager, ToastMessage *message)
        : ToastManagerEvent(toastManager), mMessage(message) {}

    ToastMessage *getMessage() const { return mMessage; }
    ToastMessageType getType() const { return static_cast<ToastMessageType>(mMessage->mType); }

    bool isCancelled() const { return mCancelled; }
    void cancel() { mCancelled = true; }
};
