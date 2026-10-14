#pragma once

#include "nmdk/Types.hpp"

#include <cstddef>
#include <span>
#include <string>
#include <string_view>

namespace nmdk {

class Patch {
  public:
    Patch() = default;
    Patch(address_t address, std::span<const std::byte> bytes, std::string name);
    Patch(address_t address, std::string_view hexBytes, std::string name);

    Patch(const Patch &) = delete;
    Patch &operator=(const Patch &) = delete;
    Patch(Patch &&other) noexcept;
    Patch &operator=(Patch &&other) noexcept;
    ~Patch();

    [[nodiscard]] bool applied() const noexcept { return mApplied; }
    [[nodiscard]] std::string_view name() const noexcept { return mName; }
    void reset() noexcept;

    static bool write(address_t address, std::span<const std::byte> bytes, std::string_view name);
    static bool write(address_t address, std::string_view hexBytes, std::string_view name);
    static bool nop(address_t address, std::size_t size) noexcept;
    static bool revert(std::string_view name) noexcept;
    static void revertAll() noexcept;

  private:
    std::string mName;
    bool mApplied{};
};

} // namespace nmdk
