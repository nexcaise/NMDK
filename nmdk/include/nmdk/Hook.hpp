#pragma once

#include "nmdk/Types.hpp"

#include <cstddef>
#include <string>
#include <string_view>

namespace nmdk {

enum class HookKind : int {
    Auto = 0,
#if defined(__aarch64__)
    Legacy = 0x1,
    Micro = 0x2,
    Nano = 0x3,
#elif defined(__x86_64__) || defined(__amd64__)
    Short = 0x1,
    Long = 0x2,
#else
    Unsupported = -1,
#endif
};

class InlineHook {
  public:
    InlineHook() = default;
    InlineHook(void *target, void *detour, HookKind kind = HookKind::Auto);

    InlineHook(const InlineHook &) = delete;
    InlineHook &operator=(const InlineHook &) = delete;
    InlineHook(InlineHook &&other) noexcept;
    InlineHook &operator=(InlineHook &&other) noexcept;
    ~InlineHook();

    [[nodiscard]] bool installed() const noexcept { return mHandle != nullptr; }
    [[nodiscard]] void *original() const noexcept { return mOriginal; }
    [[nodiscard]] void *trampoline() const noexcept;
    void reset() noexcept;

  private:
    void *mHandle{};
    void *mOriginal{};
};

class PltHook {
  public:
    PltHook() = default;
    PltHook(std::string_view module, std::string_view symbol, void *replacement);
    PltHook(const PltHook &) = delete;
    PltHook &operator=(const PltHook &) = delete;
    PltHook(PltHook &&other) noexcept;
    PltHook &operator=(PltHook &&other) noexcept;
    ~PltHook();

    [[nodiscard]] bool installed() const noexcept { return mHandle != nullptr; }
    [[nodiscard]] void *original() const noexcept { return mOriginal; }
    void reset() noexcept;

  private:
    void *mHandle{};
    void *mOriginal{};
};

} // namespace nmdk
