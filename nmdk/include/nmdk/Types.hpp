#pragma once

#include <cstddef>
#include <cstdint>

namespace nmdk {

using address_t = std::uintptr_t;

enum class ScanFlags : int {
    None = 0,
    ExecutableMemory = 1,
    WritableMemory = 1 << 1,
    ForceProtection = 1 << 3,
};

struct ModuleRegion {
    address_t begin{};
    address_t end{};
    int protection{};

    [[nodiscard]] std::size_t size() const noexcept { return end > begin ? static_cast<std::size_t>(end - begin) : 0U; }
    [[nodiscard]] bool readable() const noexcept { return (protection & 1) != 0; }
    [[nodiscard]] bool writable() const noexcept { return (protection & 2) != 0; }
    [[nodiscard]] bool executable() const noexcept { return (protection & 4) != 0; }
};

} // namespace nmdk
