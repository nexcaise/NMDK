#pragma once

#include "nmdk/Types.hpp"

#include <cstddef>
#include <span>
#include <type_traits>
#include <vector>

namespace nmdk {

class Memory {
  public:
    static bool read(address_t address, std::span<std::byte> out) noexcept;
    static bool write(address_t address, std::span<const std::byte> bytes) noexcept;

    template <typename T> static bool read(address_t address, T &value) noexcept {
        static_assert(std::is_trivially_copyable_v<T>);
        return read(address, std::as_writable_bytes(std::span{&value, 1}));
    }

    template <typename T> static bool write(address_t address, const T &value) noexcept {
        static_assert(std::is_trivially_copyable_v<T>);
        return write(address, std::as_bytes(std::span{&value, 1}));
    }

    [[nodiscard]] static std::vector<std::byte> read(address_t address, std::size_t size);
    [[nodiscard]] static address_t resolvePointerChain(address_t base, std::span<const address_t> offsets) noexcept;
    [[nodiscard]] static int protection(address_t address) noexcept;
    [[nodiscard]] static void *mapNear(address_t hint, std::size_t size, int protection) noexcept;
};

} // namespace nmdk
