#pragma once

#include <pl/Mod.hpp>
#include <nmdk/EventBus.hpp>

class NmdkExamples {
public:
    NmdkExamples();

    bool load();
    bool enable();
    bool disable();
    bool unload();

private:
    ll::mod::NativeMod* mSelf{};

    nmdk::EventBus::ListenerId mIdCreate{};
    nmdk::EventBus::ListenerId mIdAppPlatformInit{};
    nmdk::EventBus::ListenerId mIdAfterCompose{};
    nmdk::EventBus::ListenerId mIdPreGame{};
    nmdk::EventBus::ListenerId mIdSettingsOpen{};
    nmdk::EventBus::ListenerId mIdBeforeExit{};
    nmdk::EventBus::ListenerId mIdClientInit{};
    nmdk::EventBus::ListenerId mIdTouchGlyph{};
};
