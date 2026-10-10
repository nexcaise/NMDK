#pragma once

#include "mc/_HeaderOutputPredefine.h"

namespace Common {

struct BuildInfo {
  public:
    std::string mGameVersion;
    std::string mBuildId;
    std::string mCommitId;
    std::string mBranchId;
    std::string mFlavor;
    std::string mMergeBaseSha;
};

} // namespace Common
