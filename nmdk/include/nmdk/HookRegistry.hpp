#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

namespace nmdk {

enum class HookPriority : int {
    Lowest = -100,
    Low = -50,
    Normal = 0,
    High = 50,
    Highest = 100,
};

constexpr HookPriority operator+(HookPriority p, int d) noexcept {
    return static_cast<HookPriority>(static_cast<int>(p) + d);
}
constexpr HookPriority operator-(HookPriority p, int d) noexcept {
    return static_cast<HookPriority>(static_cast<int>(p) - d);
}

struct HookInfo {
    bool valid{};
    bool enabled{};
    bool plt{};
    int priority{};
    std::size_t position{};
    std::size_t chainSize{};
};

class HookRegistry {
  public:
    using HookId = std::uint64_t;
    using Setter = void (*)(void *ctx, void *original);

    static HookId install(void *target, void *detour, int priority, Setter setter, void *ctx) noexcept;
    static HookId install(void *target, void *detour, int priority, void **originalSlot) noexcept;

    static HookId installPlt(const char *moduleName, const char *symbolName, void *detour, int priority, Setter setter,
                             void *ctx) noexcept;
    static HookId installPlt(const char *moduleName, const char *symbolName, void *detour, int priority,
                             void **originalSlot) noexcept;

    static HookId install(void *target, void *detour, HookPriority priority, Setter setter, void *ctx) noexcept {
        return install(target, detour, static_cast<int>(priority), setter, ctx);
    }
    static HookId install(void *target, void *detour, HookPriority priority, void **originalSlot) noexcept {
        return install(target, detour, static_cast<int>(priority), originalSlot);
    }
    static HookId installPlt(const char *moduleName, const char *symbolName, void *detour, HookPriority priority,
                             Setter setter, void *ctx) noexcept {
        return installPlt(moduleName, symbolName, detour, static_cast<int>(priority), setter, ctx);
    }
    static HookId installPlt(const char *moduleName, const char *symbolName, void *detour, HookPriority priority,
                             void **originalSlot) noexcept {
        return installPlt(moduleName, symbolName, detour, static_cast<int>(priority), originalSlot);
    }

    static HookId pltHook(const char *moduleName, const char *symbolName, void *detour, int priority, Setter setter,
                          void *ctx) noexcept {
        return installPlt(moduleName, symbolName, detour, priority, setter, ctx);
    }
    static HookId pltHook(const char *moduleName, const char *symbolName, void *detour, int priority,
                          void **originalSlot) noexcept {
        return installPlt(moduleName, symbolName, detour, priority, originalSlot);
    }

    static bool remove(HookId id) noexcept;
    static bool remove(void *target, void *detour) noexcept;

    static bool setEnabled(HookId id, bool enabled) noexcept;

    static bool setPriority(HookId id, int priority) noexcept;
    static bool setPriority(HookId id, HookPriority priority) noexcept {
        return setPriority(id, static_cast<int>(priority));
    }

    [[nodiscard]] static HookInfo info(HookId id) noexcept;
    [[nodiscard]] static std::size_t chainSize(void *target) noexcept;
    [[nodiscard]] static bool isHooked(void *target) noexcept;
    [[nodiscard]] static std::size_t pltChainSize(const char *moduleName, const char *symbolName) noexcept;
    [[nodiscard]] static bool isPltHooked(const char *moduleName, const char *symbolName) noexcept;
};

class ScopedHook {
  public:
    ScopedHook() = default;
    explicit ScopedHook(HookRegistry::HookId id) noexcept : mId(id) {}
    ScopedHook(const ScopedHook &) = delete;
    ScopedHook &operator=(const ScopedHook &) = delete;
    ScopedHook(ScopedHook &&o) noexcept : mId(std::exchange(o.mId, 0)) {}
    ScopedHook &operator=(ScopedHook &&o) noexcept {
        if (this != &o) {
            reset();
            mId = std::exchange(o.mId, 0);
        }
        return *this;
    }
    ~ScopedHook() { reset(); }

    explicit operator bool() const noexcept { return mId != 0; }
    [[nodiscard]] HookRegistry::HookId id() const noexcept { return mId; }
    HookRegistry::HookId release() noexcept { return std::exchange(mId, 0); }

    void reset() noexcept {
        if (mId)
            HookRegistry::remove(std::exchange(mId, 0));
    }
    bool setEnabled(bool on) const noexcept { return mId && HookRegistry::setEnabled(mId, on); }
    bool setPriority(int p) const noexcept { return mId && HookRegistry::setPriority(mId, p); }
    bool setPriority(HookPriority p) const noexcept { return setPriority(static_cast<int>(p)); }

  private:
    HookRegistry::HookId mId{};
};

} // namespace nmdk
