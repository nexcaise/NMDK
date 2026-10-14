#pragma once

#include <cstddef>
#include <cstdint>

namespace nmdk {

class HookRegistry {
  public:
    using Setter = void (*)(void *ctx, void *original);

    static std::uint64_t install(void *target, void *detour, int priority, Setter setter, void *ctx) noexcept;

    static std::uint64_t install(void *target, void *detour, int priority, void **originalSlot) noexcept;

    static std::uint64_t installPlt(const char *moduleName, const char *symbolName, void *detour, int priority,
                                    Setter setter, void *ctx) noexcept;

    static std::uint64_t installPlt(const char *moduleName, const char *symbolName, void *detour, int priority,
                                    void **originalSlot) noexcept;

    static std::uint64_t pltHook(const char *moduleName, const char *symbolName, void *detour, int priority,
                                 Setter setter, void *ctx) noexcept {
        return installPlt(moduleName, symbolName, detour, priority, setter, ctx);
    }

    static std::uint64_t pltHook(const char *moduleName, const char *symbolName, void *detour, int priority,
                                 void **originalSlot) noexcept {
        return installPlt(moduleName, symbolName, detour, priority, originalSlot);
    }

    static bool remove(std::uint64_t id) noexcept;
    static bool remove(void *target, void *detour) noexcept;

    [[nodiscard]] static std::size_t chainSize(void *target) noexcept;
    [[nodiscard]] static bool isHooked(void *target) noexcept;
    [[nodiscard]] static std::size_t pltChainSize(const char *moduleName, const char *symbolName) noexcept;
    [[nodiscard]] static bool isPltHooked(const char *moduleName, const char *symbolName) noexcept;
};

} // namespace nmdk
