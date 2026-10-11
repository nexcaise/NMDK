#pragma once

#include "nmdk/Types.hpp"

#include <string_view>
#include <vector>

namespace nmdk {

class Process {
  public:
    Process() = delete;
    [[nodiscard]] static std::vector<ModuleRegion> maps(std::string_view module = {});
    [[nodiscard]] static address_t moduleBase(std::string_view module, std::string_view permissions = {}) noexcept;
    [[nodiscard]] static int protection(address_t address) noexcept;
    [[nodiscard]] static bool isReadable(address_t address) noexcept { return (protection(address) & 1) != 0; }
    [[nodiscard]] static bool isWritable(address_t address) noexcept { return (protection(address) & 2) != 0; }
    [[nodiscard]] static bool isExecutable(address_t address) noexcept { return (protection(address) & 4) != 0; }
};

} // namespace nmdk
