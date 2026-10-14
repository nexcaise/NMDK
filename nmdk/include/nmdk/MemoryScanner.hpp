#pragma once

#include "nmdk/Types.hpp"

#include <cstddef>
#include <string>
#include <string_view>

namespace nmdk {

class MemoryScanner {
  public:
    enum class Kind : int { Auto = 0, Pattern = 1, Symbol = 2 };

    [[nodiscard]] static address_t resolve(std::string_view identifier, std::string_view module,
                                           Kind kind = Kind::Auto) noexcept;

    [[nodiscard]] static address_t resolveSymbol(std::string_view symbol, std::string_view module) noexcept {
        return resolve(symbol, module, Kind::Symbol);
    }

    [[nodiscard]] static address_t resolvePattern(std::string_view pattern, std::string_view module) noexcept {
        return resolve(pattern, module, Kind::Pattern);
    }

    static void put(std::string_view identifier, std::string_view module, address_t address, Kind kind = Kind::Auto);

    [[nodiscard]] static address_t peek(std::string_view identifier, std::string_view module,
                                        Kind kind = Kind::Auto) noexcept;

    [[nodiscard]] static bool cached(std::string_view identifier, std::string_view module,
                                     Kind kind = Kind::Auto) noexcept;

    static void invalidate(std::string_view module);
    static void clear() noexcept;
    [[nodiscard]] static std::size_t size() noexcept;

    [[nodiscard]] static Kind classify(std::string_view identifier) noexcept;
    [[nodiscard]] static std::string normalize(std::string_view identifier, Kind kind);
};

} // namespace nmdk
