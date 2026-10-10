#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/common/BuildInfo.h"

namespace Common {

struct StringConstants {
  public:
    std::string mGameVersionStringNet;
    std::string mBuildIdString;
    std::string mGameVersionString;
    std::string mGameSemVerString;
    std::string mServerVersionString;
    std::string mGameDisplayVersionString;
    std::string mGameDisplayVersionStringNet;
    std::string mBuildFlavor;
    std::optional<std::string> mGameDevGitShaString;
    std::optional<std::string> mGameDevGitBranchString;
    std::optional<std::string> mMergeBaseShaString;
    Common::BuildInfo mBuildInfo;
};

} // namespace Common
