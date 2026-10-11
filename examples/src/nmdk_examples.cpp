#include "nmdk_examples.hpp"

#include <nmdk/NMDK.hpp>
#include <nmdk/events/AppPlatform.hpp>
#include <nmdk/events/ClientInstance.hpp>
#include <nmdk/events/GeneralSettingsScreenController.hpp>
#include <nmdk/events/ResourcePackManager.hpp>

NmdkExamples::NmdkExamples() : mSelf(ll::mod::NativeMod::current()) {}

bool NmdkExamples::load() {
    if (!mSelf) mSelf = ll::mod::NativeMod::current();
    if (mSelf) mSelf->getLogger().info("NmdkExamples loaded.");
    return mSelf != nullptr;
}

bool NmdkExamples::enable() {
    if (mIdCreate != 0) return true; // already enabled

    auto& bus = nmdk::getEventBus();

    // 1) Unlock the pack menu when GeneralSettingsScreenController is created.
    mIdCreate = bus.subscribe<GeneralSettingsScreenControllerOnCreateEvent>(
        [](GeneralSettingsScreenControllerOnCreateEvent& event) {
            event.unlockPackMenu();
        });

    // 2) Save AppPlatform when initialized.
    mIdAppPlatformInit = bus.subscribe<AppPlatformInitializeEvent>(
        [this](AppPlatformInitializeEvent& event) {
            mAppPlatform.store(event.mAppPlatform);
        });

    // 3) After the full stack compose is complete: suspend and then resume the app
    // (forces the game to reload) if the AppPlatform has been saved.
    mIdAfterCompose = bus.subscribe<ResourcePackManagerAfterComposeFullStackEvent>(
        [this](ResourcePackManagerAfterComposeFullStackEvent&) {
            AppPlatform* platform = mAppPlatform.load();
            if (platform != nullptr) {
                platform->_fireAppSuspended();
                platform->_fireAppResumed();
            }
        });

    // 4) While inside settings, force isPreGame = true.
    // Outside settings, the listener doesn't change anything -> the default value is used.

    // Its use is also to display the Active/Deactive button in the In game in the global resource pack settings.
    mIdPreGame = bus.subscribe<ClientInstanceIsPreGameEvent>(
        [this](ClientInstanceIsPreGameEvent& event) {
            if (mInSettings.load()) {
                event.setIsPreGame(true);
            }
        });

    // 5) Go to settings -> enable override.
    mIdSettingsOpen = bus.subscribe<GeneralSettingsScreenControllerOnOpenEvent>(
        [this](GeneralSettingsScreenControllerOnOpenEvent&) {
            mInSettings.store(true);
        });

    // 6) Exit settings -> return to default.
    mIdBeforeExit = bus.subscribe<GeneralSettingsScreenControllerBeforeExitEvent>(
        [this](GeneralSettingsScreenControllerBeforeExitEvent&) {
            mInSettings.store(false);
        });

    if (mSelf) mSelf->getLogger().info("NmdkExamples enabled.");

    return mIdCreate && mIdAppPlatformInit && mIdAfterCompose
        && mIdPreGame && mIdSettingsOpen && mIdBeforeExit;
}

bool NmdkExamples::disable() {
    auto& bus = nmdk::getEventBus();

    for (auto* id : {&mIdCreate, &mIdAppPlatformInit, &mIdAfterCompose,
                     &mIdPreGame, &mIdSettingsOpen, &mIdBeforeExit}) {
        if (*id != 0) {
            bus.unsubscribe(*id);
            *id = 0;
        }
    }

    mInSettings.store(false);
    mAppPlatform.store(nullptr);

    if (mSelf) mSelf->getLogger().info("NmdkExamples disabled.");
    return true;
}

bool NmdkExamples::unload() {
    disable();
    if (mSelf) mSelf->getLogger().info("NmdkExamples unloaded.");
    return true;
}

PL_REGISTER_MOD(NmdkExamples, NmdkExamples{});
