#pragma once

#include "mc/_HeaderOutputPredefine.h"

class GeneralSettingsScreenController {
  public:
    CLASS_FIELD(bool, mIsGlobalSettings, 0x9F8);
    CLASS_FIELD(bool, mReloadTexturePacksOnExit, 0x9F9);
    CLASS_FIELD(bool, mGlobalResourcePacksVisible, 0x9FA);
    CLASS_FIELD(bool, mLegacySyncOnly, 0x9FB);
};
