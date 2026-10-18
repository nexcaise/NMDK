#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/client/gui/ViewRequest.h"
#include "mc/deps/input/enums/ButtonState.h"
#include "mc/platform/brstd/move_only_function.h"

class ScreenController {
  public:
    enum class PreviousButtonStateRequirement : int {
        Any = 0,
        Down = 1,
        Up = 2,
        NotDown = 3,
        NotUp = 4,
    };

  public:
    void registerButtonPressedHandler(uint buttonId,
                                      ::brstd::move_only_function<::ui::ViewRequest(void *) const> callback) {
        registerButtonEventHandler(buttonId, ::ButtonState::Down,
                                   ::ScreenController::PreviousButtonStateRequirement::NotDown, std::move(callback));
    }

    [[nodiscard]] uint _getNameId(::std::string const &name) const {
        uint hash = 0x811C9DC5u;
        for (char const c : name) {
            hash = (hash ^ c) * 0x01000193u;
        }
        return hash;
    }

  public:
    void registerButtonEventHandler(uint buttonId, ::ButtonState currentState,
                                    ::ScreenController::PreviousButtonStateRequirement previousStateRequirement,
                                    ::brstd::move_only_function<::ui::ViewRequest(void *) const> callback);
};
