#pragma once

#include "nmdk/Memory.hpp"

#include <cstddef>
#include <string_view>

namespace nmdk {

class VTable {
  public:
    VTable() = delete;
    [[nodiscard]] static void **find(std::string_view module, std::string_view className) noexcept;
    [[nodiscard]] static address_t resolve(std::string_view typeInfoName, std::size_t slot,
                                           std::string_view module) noexcept;
    [[nodiscard]] static void *hook(void **table, std::size_t slot, void *replacement) noexcept;
};

class VTableHook {
  public:
    VTableHook() = default;
    VTableHook(void **table, std::size_t slot, void *replacement);
    VTableHook(const VTableHook &) = delete;
    VTableHook &operator=(const VTableHook &) = delete;
    VTableHook(VTableHook &&other) noexcept;
    VTableHook &operator=(VTableHook &&other) noexcept;
    ~VTableHook();

    [[nodiscard]] bool installed() const noexcept { return mTable && mReplacement; }
    [[nodiscard]] void *original() const noexcept { return mOriginal; }
    void reset() noexcept;

  private:
    void **mTable{};
    std::size_t mSlot{};
    void *mOriginal{};
    void *mReplacement{};
};

} // namespace nmdk
