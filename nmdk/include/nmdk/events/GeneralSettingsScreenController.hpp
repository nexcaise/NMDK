#pragma once

#include "mc/client/gui/screens/controllers/GeneralSettingsScreenController.h"

class GeneralSettingsScreenControllerEvent {
  public:
    GeneralSettingsScreenController *mGeneralSettingsScreenController;

    explicit GeneralSettingsScreenControllerEvent(GeneralSettingsScreenController *generalSettingsScreenController);
};

class GeneralSettingsScreenControllerOnCreateEvent : public GeneralSettingsScreenControllerEvent {
  public:
    GeneralSettingsScreenControllerOnCreateEvent(GeneralSettingsScreenController *generalSettingsScreenController,
                                                 bool isGlobalSettings, bool reloadTexturePacksOnExit,
                                                 bool legacySyncOnly);

    void unlockPackMenu();

    bool getIsGlobalSettings() const;
    void setIsGlobalSettings(bool isGlobalSettings);

    bool getReloadTexturePacksOnExit() const;
    void setReloadTexturePacksOnExit(bool reloadTexturePacksOnExit);

    bool getLegacySyncOnly() const;
    void setLegacySyncOnly(bool legacySyncOnly);
};

class GeneralSettingsScreenControllerBeforeExitEvent : public GeneralSettingsScreenControllerEvent {
  public:
    explicit GeneralSettingsScreenControllerBeforeExitEvent(
        GeneralSettingsScreenController *generalSettingsScreenController);
};

class GeneralSettingsScreenControllerAfterExitEvent : public GeneralSettingsScreenControllerEvent {
  public:
    explicit GeneralSettingsScreenControllerAfterExitEvent(
        GeneralSettingsScreenController *generalSettingsScreenController);
};

class GeneralSettingsScreenControllerOnOpenEvent : public GeneralSettingsScreenControllerEvent {
  public:
    explicit GeneralSettingsScreenControllerOnOpenEvent(
        GeneralSettingsScreenController *generalSettingsScreenController);
};
