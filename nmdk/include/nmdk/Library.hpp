#pragma once

#include "nmdk/Types.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace nmdk {

class Library {
  public:
    Library();
    explicit Library(std::string name);
    ~Library();

    Library(const Library &) = default;
    Library &operator=(const Library &) = default;
    Library(Library &&) noexcept = default;
    Library &operator=(Library &&) noexcept = default;

    [[nodiscard]] const std::string &name() const noexcept { return mName; }
    [[nodiscard]] address_t base(std::string_view permissions = {}) const noexcept;
    [[nodiscard]] std::vector<ModuleRegion> regions() const;
    [[nodiscard]] explicit operator bool() const noexcept { return base() != 0; }

  private:
    std::string mName;
};

} // namespace nmdk
