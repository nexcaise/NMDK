#include "nmdk_examples.hpp"
#include "NexLog.hpp"

#include <atomic>
#include <string>

#include <nmdk/NMDK.hpp>
#include <nmdk/events/AppPlatform.hpp>
#include <nmdk/events/ClientInstance.hpp>
#include <nmdk/events/GeneralSettingsScreenController.hpp>
#include <nmdk/events/Input.hpp>
#include <nmdk/events/ResourcePackManager.hpp>

#include "mc/deps/application/AppPlatform.h"
#include "mc/deps/core/resource/ResourceLocation.h"
#include "mc/deps/input/enums/ButtonState.h"
#include "mc/resources/ResourcePackManager.h"
#include "mc/world/actor/Actor.h"

static std::atomic<AppPlatform*>   g_appPlatform{nullptr};
static std::atomic<ClientInstance*> g_clientInstance{nullptr};
static std::atomic<bool>           g_inSettings{false};

static const char* touchGlyphName(MinecraftInputType type) {
    switch (type) {
    case MinecraftInputType::Jump:          return "Jump";
    case MinecraftInputType::Sneak:         return "Sneak";
    case MinecraftInputType::BuildUse:      return "BuildUse";
    case MinecraftInputType::AttackDestroy: return "AttackDestroy";
    case MinecraftInputType::PickBlock:     return "PickBlock";
    case MinecraftInputType::Pause:         return "Pause";
    case MinecraftInputType::Chat:          return "Chat";
    case MinecraftInputType::Emote:         return "Emote";
    case MinecraftInputType::Perspective:   return "Perspective";
    default:                                  return "Unknown";
    }
}

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
        [](AppPlatformInitializeEvent& event) {
            g_appPlatform.store(event.mAppPlatform);
        });

    // 3) Save ClientInstance (used for testing resource pack).
    mIdClientInit = bus.subscribe<ClientInstanceInitFinishedEvent>(
        [](ClientInstanceInitFinishedEvent& event) {
            if (g_clientInstance.load() == nullptr) g_clientInstance.store(event.mClient);
        });

    // 4) After full stack compose is complete: suspend then resume app for reloading the game.
    mIdAfterCompose = bus.subscribe<ResourcePackManagerAfterComposeFullStackEvent>(
        [](ResourcePackManagerAfterComposeFullStackEvent&) {
            AppPlatform* platform = g_appPlatform.load();
            ClientInstance* client = g_clientInstance.load();
            if (platform != nullptr && client != nullptr && client->getLocalPlayer() != nullptr) {
                platform->_fireAppSuspended();
                platform->_fireAppResumed();
            }
        });

    // 5) While inside settings, force isPreGame = true.
    // Outside settings, it doesn't change anything -> the default value is used.
    // Useful for making Active/Deactive Buttons appear ingame in global packs settings
    mIdPreGame = bus.subscribe<ClientInstanceIsPreGameEvent>(
        [](ClientInstanceIsPreGameEvent& event) {
            if (g_inSettings.load()) {
                event.setIsPreGame(true);
            }
        });

    // 6) Go to settings: enable override isPreGame + test read resource pack.
    mIdSettingsOpen = bus.subscribe<GeneralSettingsScreenControllerOnOpenEvent>(
        [](GeneralSettingsScreenControllerOnOpenEvent&) {
            g_inSettings.store(true);

            ClientInstance* client = g_clientInstance.load();
            if (client != nullptr) {
                ResourcePackManager& resourcePackManager = client->getResourcePackManager();
                std::string textOut;
                bool loaded = resourcePackManager.load(ResourceLocation("nmdk_test/text.txt"), textOut);
                if (loaded) {
                    LOGI("NmdkExamples resource pack text: %s", textOut.c_str());
                }
            }
        });

    // 7) Exit settings: return to default.
    mIdBeforeExit = bus.subscribe<GeneralSettingsScreenControllerBeforeExitEvent>(
        [](GeneralSettingsScreenControllerBeforeExitEvent&) {
            g_inSettings.store(false);
        });

    // 8) Log every touch glyph button press/release (type + state).
    mIdTouchGlyph = bus.subscribe<TouchButtonGlyphEvent>(
        [](TouchButtonGlyphEvent& event) {
            ButtonState state = event.mIsDown ? ButtonState::Down : ButtonState::Up;
            LOGI("TouchButtonGlyphEvent type=%s (%u) state=%s (%d)",
                 touchGlyphName(event.mInputType),
                 static_cast<unsigned int>(event.mInputType),
                 event.mIsDown ? "Down" : "Up",
                 static_cast<int>(state));
        });

    if (mSelf) mSelf->getLogger().info("NmdkExamples enabled.");

    return mIdCreate && mIdAppPlatformInit && mIdClientInit && mIdAfterCompose
        && mIdPreGame && mIdSettingsOpen && mIdBeforeExit && mIdTouchGlyph;
}

bool NmdkExamples::disable() {
    auto& bus = nmdk::getEventBus();

    for (auto* id : {&mIdCreate, &mIdAppPlatformInit, &mIdClientInit, &mIdAfterCompose,
                     &mIdPreGame, &mIdSettingsOpen, &mIdBeforeExit, &mIdTouchGlyph}) {
        if (*id != 0) {
            bus.unsubscribe(*id);
            *id = 0;
        }
    }

    g_inSettings.store(false);
    g_appPlatform.store(nullptr);
    g_clientInstance.store(nullptr);

    if (mSelf) mSelf->getLogger().info("NmdkExamples disabled.");
    return true;
}

bool NmdkExamples::unload() {
    disable();
    if (mSelf) mSelf->getLogger().info("NmdkExamples unloaded.");
    return true;
}

PL_REGISTER_MOD(NmdkExamples, NmdkExamples{});
