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
    nmdk::EventBus::ListenerId mListenerIdClientUpdate{};
    nmdk::EventBus::ListenerId mListenerIdSettingsOpen{};
};
