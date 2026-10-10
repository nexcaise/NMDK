#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/client/gui/screens/interfaces/ISceneStack.h"
#include "mc/platform/brstd/function_ref.h"

class AbstractScene;
class GuiData;
class IOptionRegistry;
class ResourcePackManager;

class ClientInstance {
  public:
    GuiData *getGuiData();

  public:
    ResourcePackManager &getResourcePackManager() const;
    ::IOptionRegistry &getOptions();
    Bedrock::NotNullNonOwnerPtr<ISceneStack> getCurrentSceneStack();
    void forEachScreen(::brstd::function_ref<bool(::AbstractScene &)> callback, bool topDown);
};
