#include "nmdk_examples.hpp"
#include "NexLog.hpp"

#include <nmdk/NMDK.hpp>
#include <nmdk/events/ClientInstance.hpp>
#include <nmdk/events/GeneralSettingsScreenController.hpp>

#include "mc/deps/core/resource/ResourceLocation.h"
#include "mc/resources/ResourcePackManager.h"

static ClientInstance* g_clientInstance = nullptr;

NmdkExamples::NmdkExamples() : mSelf(ll::mod::NativeMod::current()) {}

bool NmdkExamples::load() {
    if (!mSelf) {
        mSelf = ll::mod::NativeMod::current();
    }

    if (mSelf) {
        mSelf->getLogger().info("NmdkExamples loaded.");
    }

    return mSelf != nullptr;
}

bool NmdkExamples::enable() {
    if (mListenerIdSettingsOpen != 0 || mListenerIdClientUpdate != 0) {
        return true;
    }

    mListenerIdSettingsOpen = nmdk::getEventBus().subscribe<GeneralSettingsScreenControllerOnOpenEvent>(
        [this](GeneralSettingsScreenControllerOnOpenEvent&) {
            if (g_clientInstance != nullptr) {
                ResourcePackManager& resourcePackManager = g_clientInstance->getResourcePackManager();
                std::string textOut;
                bool loaded = resourcePackManager.load(ResourceLocation("nmdk_test/text.txt"), textOut);
                if (loaded) {
                    LOGI("NmdkExamples resource pack text: %s", textOut.c_str());
                }
            }
        }
    );

    mListenerIdClientUpdate = nmdk::getEventBus().subscribe<ClientInstanceInitFinishedEvent>(
        [this](ClientInstanceInitFinishedEvent& event) {
            if(g_clientInstance == nullptr) g_clientInstance = event.mClient;
        }
    );

    if (mSelf) {
        mSelf->getLogger().info("NmdkExamples enabled. Listener ID ClientUpdate: {}", mListenerIdClientUpdate);
        mSelf->getLogger().info("NmdkExamples enabled. Listener ID SettingsOpen: {}", mListenerIdSettingsOpen);
    }

    return mListenerIdSettingsOpen != 0 && mListenerIdClientUpdate != 0;
}

bool NmdkExamples::disable() {
    if (mListenerIdSettingsOpen != 0 || mListenerIdClientUpdate != 0) {
        nmdk::getEventBus().unsubscribe(mListenerIdSettingsOpen);
        mListenerIdSettingsOpen = 0;
        nmdk::getEventBus().unsubscribe(mListenerIdClientUpdate);
        mListenerIdClientUpdate = 0;
    }

    if (mSelf) {
        mSelf->getLogger().info("NmdkExamples disabled.");
    }

    return true;
}

bool NmdkExamples::unload() {
    disable();

    if (mSelf) {
        mSelf->getLogger().info("NmdkExamples unloaded.");
    }

    return true;
}

PL_REGISTER_MOD(NmdkExamples, NmdkExamples{});
