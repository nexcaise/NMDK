#pragma once

#include "mc/client/gui/screens/controllers/GeneralSettingsScreenController.h"

class GeneralSettingsScreenControllerEvent {
  public:
    GeneralSettingsScreenController *mGeneralSettingsScreenController;

    explicit GeneralSettingsScreenControllerEvent(GeneralSettingsScreenController *controller)
        : mGeneralSettingsScreenController(controller) {}
};

class GeneralSettingsScreenControllerOnCreateEvent : public GeneralSettingsScreenControllerEvent {
  private:
    bool mIsGlobalSettings;
    bool mReloadTexturePacksOnExit;
    bool mLegacySyncOnly;

  public:
    GeneralSettingsScreenControllerOnCreateEvent(GeneralSettingsScreenController *controller, bool isGlobalSettings,
                                                 bool reloadTexturePacksOnExit, bool legacySyncOnly)
        : GeneralSettingsScreenControllerEvent(controller), mIsGlobalSettings(isGlobalSettings),
          mReloadTexturePacksOnExit(reloadTexturePacksOnExit), mLegacySyncOnly(legacySyncOnly) {}

    void unlockPackMenu() {
        mIsGlobalSettings = true;
        mReloadTexturePacksOnExit = true;
    }

    bool getIsGlobalSettings() const { return mIsGlobalSettings; }
    void setIsGlobalSettings(bool v) { mIsGlobalSettings = v; }

    bool getReloadTexturePacksOnExit() const { return mReloadTexturePacksOnExit; }
    void setReloadTexturePacksOnExit(bool v) { mReloadTexturePacksOnExit = v; }

    bool getLegacySyncOnly() const { return mLegacySyncOnly; }
    void setLegacySyncOnly(bool v) { mLegacySyncOnly = v; }
};

class GeneralSettingsScreenControllerBeforeExitEvent : public GeneralSettingsScreenControllerEvent {
  public:
    explicit GeneralSettingsScreenControllerBeforeExitEvent(GeneralSettingsScreenController *controller)
        : GeneralSettingsScreenControllerEvent(controller) {}
};

class GeneralSettingsScreenControllerAfterExitEvent : public GeneralSettingsScreenControllerEvent {
  public:
    explicit GeneralSettingsScreenControllerAfterExitEvent(GeneralSettingsScreenController *controller)
        : GeneralSettingsScreenControllerEvent(controller) {}
};

class GeneralSettingsScreenControllerOnOpenEvent : public GeneralSettingsScreenControllerEvent {
  public:
    explicit GeneralSettingsScreenControllerOnOpenEvent(GeneralSettingsScreenController *controller)
        : GeneralSettingsScreenControllerEvent(controller) {}
};
