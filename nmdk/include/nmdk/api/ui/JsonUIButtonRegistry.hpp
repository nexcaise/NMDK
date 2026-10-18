#pragma once

#include "nmdk/api/Macro.h"

#include "mc/client/gui/ViewRequest.h"
#include "mc/client/gui/screens/ScreenController.h"
#include "mc/deps/input/enums/ButtonState.h"

#include <functional>
#include <string>

namespace nmdk::ui {

// Required previous state of the button (alias of ScreenController::PreviousButtonStateRequirement).
using PreviousButtonState = ::ScreenController::PreviousButtonStateRequirement;

// Button callback. The `void*` argument is the same one the game passes to its own handlers.
using ButtonCallback = std::function<::ui::ViewRequest(void *)>;

/*
 * Global JsonUI button registry owned by NMDK.
 *
 * A mod registers the name of a button (`button.xxx` in the JSON UI) once; NMDK attaches the
 * handler to every menu ScreenController (MainMenu / StartMenu) when that screen opens, so mods
 * do not have to hook ScreenController themselves.
 */
class MCAPI JsonUIButtonRegistry {
  public:
    static JsonUIButtonRegistry &getInstance() noexcept;

    // Registers (or replaces) the handler for button `name`. Takes effect the next time a screen opens.
    void registerButton(std::string name, ::ButtonState state, PreviousButtonState previousState,
                        ButtonCallback callback);

    // Removes the handler for button `name`. Screens that are already open are not affected.
    void unregisterButton(std::string name);

  private:
    JsonUIButtonRegistry() = default;
    ~JsonUIButtonRegistry() = default;
    JsonUIButtonRegistry(const JsonUIButtonRegistry &) = delete;
    JsonUIButtonRegistry &operator=(const JsonUIButtonRegistry &) = delete;
};

} // namespace nmdk::ui
