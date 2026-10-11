#pragma once

#include "nmdk/Types.hpp"

#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace nmdk {

class Signature {
  public:
    Signature() = delete;
    [[nodiscard]] static address_t findFirst(std::string_view pattern, std::string_view module,
                                             int flags = static_cast<int>(ScanFlags::None)) noexcept;
    [[nodiscard]] static address_t findCode(std::string_view pattern, std::string_view module) noexcept {
        return findFirst(pattern, module, static_cast<int>(ScanFlags::ExecutableMemory));
    }
    [[nodiscard]] static std::vector<address_t> findAll(std::string_view pattern, std::string_view module,
                                                        int flags = static_cast<int>(ScanFlags::None));
    [[nodiscard]] static std::vector<address_t> findAll(std::span<const std::string_view> patterns,
                                                        std::string_view module,
                                                        int flags = static_cast<int>(ScanFlags::None));
    [[nodiscard]] static std::unordered_map<std::string, address_t>
    findMany(std::span<const std::string_view> patterns, std::string_view module,
             int flags = static_cast<int>(ScanFlags::None));
};

} // namespace nmdk
