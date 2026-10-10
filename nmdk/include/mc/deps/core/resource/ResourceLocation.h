#pragma once

#include "mc/_HeaderOutputPredefine.h"

#include "mc/deps/core/resource/ResourceFileSystem.h"

class ResourceLocation {
  public:
    ResourceFileSystem mFileSystem;
    std::string mPath;
    uint64 mPathHash;
    uint64 mFullHash;

  public:
    ResourceLocation() : ResourceLocation("", ResourceFileSystem::UserPackage) {}

    explicit ResourceLocation(const std::string &path, ResourceFileSystem fs = ResourceFileSystem::UserPackage)
        : mFileSystem(fs), mPath(path), mPathHash(computePathHash(path)),
          mFullHash(mPathHash ^ static_cast<uint64>(mFileSystem)) {}

    explicit ResourceLocation(const char *path, ResourceFileSystem fs = ResourceFileSystem::UserPackage)
        : ResourceLocation(path ? std::string(path) : std::string(), fs) {}

  private:
    static uint64 computePathHash(std::string_view path) {
        constexpr uint64 kOffset = 1469598103934665603ULL;
        constexpr uint64 kPrime = 1099511628211ULL;

        uint64 hash = kOffset;
        for (unsigned char ch : path) {
            hash = static_cast<uint64>(ch) ^ (kPrime * hash);
        }
        return hash;
    }
};
