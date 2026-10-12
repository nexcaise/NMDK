#pragma once

#include <nmdk/api/Macro.h>
#include <nmdk/api/Types.h>

#if defined(NXC_CXX) && NXC_CXX
#include <expected>
#endif

#include <algorithm>
#include <android/dlext.h>
#include <android/log.h>
#include <any>
#include <array>
#include <atomic>
#include <bitset>
#include <cctype>
#include <cerrno>
#include <chrono>
#include <clocale>
#include <cmath>
#include <complex>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>
#include <deque>
#include <dirent.h>
#include <dlfcn.h>
#include <elf.h>
#include <exception>
#include <fcntl.h>
#include <filesystem>
#include <forward_list>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <jni.h>
#include <limits>
#include <link.h>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <numbers>
#include <optional>
#include <ostream>
#include <pthread.h>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <shared_mutex>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <string_view>
#include <sys/mman.h>
#include <sys/stat.h>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#if __has_include(<entt/entt.hpp>)
#include <entt/entt.hpp>
#define NMDK_HAS_ENTT 1
#else
#define NMDK_HAS_ENTT 0
#endif

namespace fs = std::filesystem;
using scsclock = std::chrono::steady_clock;

template <std::size_t IIdx, typename TRet, typename... TArgs>
static TRet callVirtualFunc(void *thisptr, TArgs... args) {
    using Fn = TRet (*)(void *, TArgs...);
    return (*reinterpret_cast<Fn **>(thisptr))[IIdx](thisptr, args...);
}

template <std::size_t IIdx, typename TRet, typename... TArgs>
static TRet callVirtualFunc(const void *thisptr, TArgs... args) {
    using Fn = TRet (*)(void *, TArgs...);
    auto ptr = const_cast<void *>(thisptr);
    return (*reinterpret_cast<Fn **>(ptr))[IIdx](ptr, args...);
}

struct Vec2 final {
    float x;
    float y;

    constexpr Vec2() : x(0.f), y(0.f) {}

    constexpr Vec2(float x, float y) : x(x), y(y) {}

    constexpr Vec2 operator-(const Vec2 &rhs) const { return {x - rhs.x, y - rhs.y}; }

    constexpr Vec2 operator+(const Vec2 &rhs) const { return {x + rhs.x, y + rhs.y}; }

    constexpr Vec2 operator/(const Vec2 &rhs) const { return {x / rhs.x, y / rhs.y}; }

    constexpr Vec2 operator*(const Vec2 &rhs) const { return {x * rhs.x, y * rhs.y}; }

    constexpr bool operator==(const Vec2 &rhs) const { return x == rhs.x && y == rhs.y; }

    constexpr bool operator!=(const Vec2 &rhs) const { return !(*this == rhs); }

    float magnitude() const { return std::sqrt(x * x + y * y); }
};

namespace Bedrock {

class EnableNonOwnerReferences {
  public:
    struct ControlBlock {
      public:
        bool mIsValid;
    };

  public:
    std::shared_ptr<Bedrock::EnableNonOwnerReferences::ControlBlock> mControlBlock;

  public:
};

template <typename T> class NonOwnerPointer {
  public:
    std::shared_ptr<::Bedrock::EnableNonOwnerReferences::ControlBlock> mControlBlock;
    T *mPointer{};

    NonOwnerPointer(std::shared_ptr<::Bedrock::EnableNonOwnerReferences::ControlBlock> cb, T *p)
        : mControlBlock(std::move(cb)), mPointer(p) {}

    template <typename U = T>
        requires std::is_convertible_v<U *, T *>
    NonOwnerPointer(U const &t)
        : NonOwnerPointer(static_cast<::Bedrock::EnableNonOwnerReferences const &>(t).mControlBlock,
                          const_cast<T *>(std::addressof(t))) {}
    template <typename U = T>
        requires std::is_convertible_v<U *, T *>
    NonOwnerPointer(NonOwnerPointer<U> const &p) : NonOwnerPointer(p.mControlBlock, p.mPointer) {}
    template <typename U = T>
        requires std::is_convertible_v<U *, T *>
    NonOwnerPointer(NonOwnerPointer<U> &&p)
        : NonOwnerPointer(std::move(p.mControlBlock), std::exchange(p.mPointer, nullptr)) {}

    NonOwnerPointer() noexcept {}
    NonOwnerPointer(std::nullptr_t) noexcept {}
    T *get() const {
        if (mControlBlock && mControlBlock->mIsValid) {
            return mPointer;
        }
        return nullptr;
    }
    explicit operator bool() const noexcept { return get() != nullptr; }

    [[nodiscard]] constexpr operator T *() const { return get(); }
    [[nodiscard]] constexpr T *operator->() const { return get(); }
    [[nodiscard]] constexpr T &operator*() const { return *get(); }

    [[nodiscard]] bool operator==(nullptr_t) noexcept { return get() == nullptr; }
    template <class U> [[nodiscard]] bool operator==(NonOwnerPointer<U> const &r) noexcept { return get() == r.get(); }
    template <class U> [[nodiscard]] bool operator<=>(NonOwnerPointer<U> const &r) noexcept {
        return get() <=> r.get();
    }
};

template <typename T> class NotNull {
  private:
    T mValue;

  public:
    constexpr NotNull(T const &value) : mValue(value) {}

    constexpr T const &get() const noexcept { return mValue; }

    constexpr operator T const &() const noexcept { return mValue; }

    constexpr T const *operator->() const noexcept { return std::addressof(mValue); }

    constexpr T const &operator*() const noexcept { return mValue; }
};

template <typename T> using NotNullNonOwnerPtr = NotNull<Bedrock::NonOwnerPointer<T>>;

}; // namespace Bedrock
