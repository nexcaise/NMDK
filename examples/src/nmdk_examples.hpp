#pragma once

#include <atomic>

#include <pl/Mod.hpp>
#include <nmdk/EventBus.hpp>

class AppPlatform;

class NmdkExamples {
public:
    NmdkExamples();

    bool load();
    bool enable();
    bool disable();
    bool unload();

private:
    ll::mod::NativeMod* mSelf{};

    // Listener IDs
    nmdk::EventBus::ListenerId mIdCreate{};
    nmdk::EventBus::ListenerId mIdAppPlatformInit{};
    nmdk::EventBus::ListenerId mIdAfterCompose{};
    nmdk::EventBus::ListenerId mIdPreGame{};
    nmdk::EventBus::ListenerId mIdSettingsOpen{};
    nmdk::EventBus::ListenerId mIdBeforeExit{};

    // State (read/written from different threads -> atomic)
    std::atomic<AppPlatform*> mAppPlatform{nullptr};
    std::atomic<bool>         mInSettings{false};
};
