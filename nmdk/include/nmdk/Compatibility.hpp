#pragma once

#include "nmdk/HookRegistry.hpp"
#include "nmdk/MemoryScanner.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <utility>

namespace nmdk::memory {

using FuncPtr = void *;

using HookPriority = ::nmdk::HookPriority;

template <typename T> struct IsConstMemberFun : std::false_type {};
template <typename T, typename Ret, typename... Args>
struct IsConstMemberFun<Ret (T::*)(Args...) const> : std::true_type {};
template <typename T> inline constexpr bool IsConstMemberFunV = IsConstMemberFun<T>::value;

template <typename T> struct AddConstAtMemberFun {
    using type = T;
};
template <typename T, typename Ret, typename... Args> struct AddConstAtMemberFun<Ret (T::*)(Args...)> {
    using type = Ret (T::*)(Args...) const;
};
template <typename T> using AddConstAtMemberFunT = typename AddConstAtMemberFun<T>::type;
template <typename T, typename U>
using AddConstAtMemberFunIfOriginIs = std::conditional_t<IsConstMemberFunV<U>, AddConstAtMemberFunT<T>, T>;

template <typename T>
concept FuncPtrType = std::is_function_v<std::remove_pointer_t<T>> || std::is_member_function_pointer_v<T>;

template <typename T>
    requires(sizeof(T) == sizeof(FuncPtr))
inline FuncPtr toFuncPtr(T value) noexcept {
    FuncPtr fp;
    std::memcpy(&fp, &value, sizeof(fp));
    return fp;
}

template <typename T>
    requires(std::is_member_function_pointer_v<T> && sizeof(T) == sizeof(FuncPtr) + sizeof(std::ptrdiff_t))
inline FuncPtr toFuncPtr(T value) noexcept {
    FuncPtr fp;
    std::memcpy(&fp, &value, sizeof(fp));
    return fp;
}

template <typename T>
    requires(std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>)
inline T fromFuncPtr(FuncPtr value) noexcept {
    return reinterpret_cast<T>(value);
}

template <typename T>
    requires(std::is_member_function_pointer_v<T> && sizeof(T) == sizeof(FuncPtr) + sizeof(std::ptrdiff_t))
inline T fromFuncPtr(FuncPtr value) noexcept {
    struct Raw {
        FuncPtr fp;
        std::ptrdiff_t offset;
    } raw{value, 0};
    T result;
    std::memcpy(&result, &raw, sizeof(result));
    return result;
}

FuncPtr resolveSignature(const char *identifier, const char *moduleName) noexcept;

template <typename T> inline FuncPtr resolveIdentifier(const char *identifier, const char *moduleName) noexcept {
    (void)sizeof(T);
    return resolveSignature(identifier, moduleName);
}

template <typename T>
inline FuncPtr resolveIdentifier(std::initializer_list<const char *> identifiers, const char *moduleName) noexcept {
    (void)sizeof(T);
    for (const char *identifier : identifiers) {
        if (auto result = resolveSignature(identifier, moduleName))
            return result;
    }
    return nullptr;
}

template <typename T> inline FuncPtr resolveIdentifier(uintptr_t address, const char *) noexcept {
    (void)sizeof(T);
    return reinterpret_cast<FuncPtr>(address);
}

template <typename T>
    requires(FuncPtrType<T>)
inline FuncPtr resolveIdentifier(T identifier, const char *) noexcept {
    return toFuncPtr(identifier);
}

template <typename T> inline FuncPtr resolveIdentifier(FuncPtr address, const char *) noexcept {
    (void)sizeof(T);
    return address;
}

template <typename T> inline T fromOriginal(void *original) noexcept { return fromFuncPtr<T>(original); }

template <typename T> struct HookAutoRegister {
    HookAutoRegister() { T::hook(); }
    ~HookAutoRegister() { T::unhook(); }
    static int hook() { return T::hook(); }
    static bool unhook() { return T::unhook(); }
};

} // namespace nmdk::memory

#ifndef VA_EXPAND
#define VA_EXPAND(...) __VA_ARGS__
#endif

#define NMDK_HOOK_IMPL(REGISTER, FUNC_PTR, STATIC, CALL, DEF_TYPE, TYPE, PRIORITY, IDENTIFIER, MODULE, RET_TYPE, ...)  \
    struct DEF_TYPE TYPE {                                                                                             \
        using FuncPtr = ::nmdk::memory::FuncPtr;                                                                       \
        using HookPriority = ::nmdk::memory::HookPriority;                                                             \
        using OriginFuncType =                                                                                         \
            ::nmdk::memory::AddConstAtMemberFunIfOriginIs<RET_TYPE FUNC_PTR(__VA_ARGS__), decltype(IDENTIFIER)>;       \
                                                                                                                       \
        inline static FuncPtr target{};                                                                                \
        inline static OriginFuncType originFunc{};                                                                     \
        inline static std::uint64_t hookId{};                                                                          \
                                                                                                                       \
        template <typename... Args> STATIC RET_TYPE origin(Args &&...params) {                                         \
            return CALL(std::forward<Args>(params)...);                                                                \
        }                                                                                                              \
                                                                                                                       \
        STATIC RET_TYPE detour(__VA_ARGS__);                                                                           \
                                                                                                                       \
        static void setOriginal(void *, void *next) {                                                                  \
            originFunc = ::nmdk::memory::fromOriginal<OriginFuncType>(next);                                           \
        }                                                                                                              \
                                                                                                                       \
        static bool installed() noexcept { return hookId != 0; }                                                       \
        static bool enabled() noexcept { return hookId != 0 && ::nmdk::HookRegistry::info(hookId).enabled; }           \
        static bool setEnabled(bool on) noexcept {                                                                     \
            return hookId != 0 && ::nmdk::HookRegistry::setEnabled(hookId, on);                                        \
        }                                                                                                              \
        static bool setPriority(HookPriority priority) noexcept {                                                      \
            return hookId != 0 && ::nmdk::HookRegistry::setPriority(hookId, priority);                                 \
        }                                                                                                              \
                                                                                                                       \
        static int hook() {                                                                                            \
            if (installed())                                                                                           \
                return 0;                                                                                              \
            target = ::nmdk::memory::resolveIdentifier<OriginFuncType>(IDENTIFIER, MODULE);                            \
            if (!target)                                                                                               \
                return -1;                                                                                             \
            hookId =                                                                                                   \
                ::nmdk::HookRegistry::install(reinterpret_cast<void *>(target),                                        \
                                              reinterpret_cast<void *>(::nmdk::memory::toFuncPtr(&DEF_TYPE::detour)),  \
                                              static_cast<int>(PRIORITY), &DEF_TYPE::setOriginal, nullptr);            \
            if (!hookId) {                                                                                             \
                target = nullptr;                                                                                      \
                return -2;                                                                                             \
            }                                                                                                          \
            return 0;                                                                                                  \
        }                                                                                                              \
                                                                                                                       \
        static bool unhook() {                                                                                         \
            if (!hookId)                                                                                               \
                return false;                                                                                          \
            ::nmdk::HookRegistry::remove(hookId);                                                                      \
            hookId = 0;                                                                                                \
                                                                                                                       \
            target = nullptr;                                                                                          \
            return true;                                                                                               \
        }                                                                                                              \
    };                                                                                                                 \
    REGISTER;                                                                                                          \
    RET_TYPE DEF_TYPE::detour(__VA_ARGS__)

#define NMDK_AUTO_REG_HOOK_IMPL(FUNC_PTR, STATIC, CALL, DEF_TYPE, ...)                                                 \
    VA_EXPAND(NMDK_HOOK_IMPL(inline ::nmdk::memory::HookAutoRegister<DEF_TYPE> DEF_TYPE##AutoRegister, FUNC_PTR,       \
                             STATIC, CALL, DEF_TYPE, __VA_ARGS__))

#define NMDK_MANUAL_REG_HOOK_IMPL(...) VA_EXPAND(NMDK_HOOK_IMPL(, __VA_ARGS__))

#define NMDK_STATIC_HOOK_IMPL(...) VA_EXPAND(NMDK_MANUAL_REG_HOOK_IMPL((*), static, originFunc, __VA_ARGS__))

#define NMDK_AUTO_STATIC_HOOK_IMPL(...) VA_EXPAND(NMDK_AUTO_REG_HOOK_IMPL((*), static, originFunc, __VA_ARGS__))

#define NMDK_INSTANCE_HOOK_IMPL(DEF_TYPE, ...)                                                                         \
    VA_EXPAND(NMDK_MANUAL_REG_HOOK_IMPL((DEF_TYPE::*), , (this->*originFunc), DEF_TYPE, __VA_ARGS__))

#define NMDK_AUTO_INSTANCE_HOOK_IMPL(DEF_TYPE, ...)                                                                    \
    VA_EXPAND(NMDK_AUTO_REG_HOOK_IMPL((DEF_TYPE::*), , (this->*originFunc), DEF_TYPE, __VA_ARGS__))

#define NMDK_TYPED_STATIC_HOOK(DefType, type, priority, identifier, module, Ret, ...)                                  \
    VA_EXPAND(NMDK_STATIC_HOOK_IMPL(DefType, : public type, priority, identifier, module, Ret, __VA_ARGS__))

#define NMDK_STATIC_HOOK(DefType, priority, identifier, module, Ret, ...)                                              \
    VA_EXPAND(NMDK_STATIC_HOOK_IMPL(DefType, , priority, identifier, module, Ret, __VA_ARGS__))

#define NMDK_AUTO_TYPED_STATIC_HOOK(DefType, type, priority, identifier, module, Ret, ...)                             \
    VA_EXPAND(NMDK_AUTO_STATIC_HOOK_IMPL(DefType, : public type, priority, identifier, module, Ret, __VA_ARGS__))

#define NMDK_AUTO_STATIC_HOOK(DefType, priority, identifier, module, Ret, ...)                                         \
    VA_EXPAND(NMDK_AUTO_STATIC_HOOK_IMPL(DefType, , priority, identifier, module, Ret, __VA_ARGS__))

#define NMDK_TYPED_HOOK(DEF_TYPE, PRIORITY, TYPE, IDENTIFIER, MODULE, RET_TYPE, ...)                                   \
    VA_EXPAND(NMDK_INSTANCE_HOOK_IMPL(DEF_TYPE, : public TYPE, PRIORITY, IDENTIFIER, MODULE, RET_TYPE, __VA_ARGS__))

#define NMDK_INSTANCE_HOOK(DEF_TYPE, PRIORITY, IDENTIFIER, MODULE, RET_TYPE, ...)                                      \
    VA_EXPAND(NMDK_INSTANCE_HOOK_IMPL(DEF_TYPE, , PRIORITY, IDENTIFIER, MODULE, RET_TYPE, __VA_ARGS__))

#define NMDK_AUTO_TYPED_INSTANCE_HOOK(DEF_TYPE, PRIORITY, TYPE, IDENTIFIER, MODULE, RET_TYPE, ...)                     \
    VA_EXPAND(                                                                                                         \
        NMDK_AUTO_INSTANCE_HOOK_IMPL(DEF_TYPE, : public TYPE, PRIORITY, IDENTIFIER, MODULE, RET_TYPE, __VA_ARGS__))

#define NMDK_AUTO_INSTANCE_HOOK(DEF_TYPE, PRIORITY, IDENTIFIER, MODULE, RET_TYPE, ...)                                 \
    VA_EXPAND(NMDK_AUTO_INSTANCE_HOOK_IMPL(DEF_TYPE, , PRIORITY, IDENTIFIER, MODULE, RET_TYPE, __VA_ARGS__))

#define NMDK_PLT_HOOK_IMPL(REGISTER, DEF_TYPE, PRIORITY, SYMBOL, MODULE, RET_TYPE, ...)                                \
    struct DEF_TYPE {                                                                                                  \
        using FuncPtr = ::nmdk::memory::FuncPtr;                                                                       \
        using HookPriority = ::nmdk::memory::HookPriority;                                                             \
        using OriginFuncType = RET_TYPE (*)(__VA_ARGS__);                                                              \
        inline static OriginFuncType originFunc{};                                                                     \
        inline static std::uint64_t hookId{};                                                                          \
        template <typename... Args> static RET_TYPE origin(Args &&...params) {                                         \
            return originFunc(std::forward<Args>(params)...);                                                          \
        }                                                                                                              \
        static RET_TYPE detour(__VA_ARGS__);                                                                           \
        static void setOriginal(void *, void *next) {                                                                  \
            originFunc = ::nmdk::memory::fromOriginal<OriginFuncType>(next);                                           \
        }                                                                                                              \
        static bool installed() noexcept { return hookId != 0; }                                                       \
        static bool enabled() noexcept { return hookId != 0 && ::nmdk::HookRegistry::info(hookId).enabled; }           \
        static bool setEnabled(bool on) noexcept {                                                                     \
            return hookId != 0 && ::nmdk::HookRegistry::setEnabled(hookId, on);                                        \
        }                                                                                                              \
        static bool setPriority(HookPriority priority) noexcept {                                                      \
            return hookId != 0 && ::nmdk::HookRegistry::setPriority(hookId, priority);                                 \
        }                                                                                                              \
        static int hook() {                                                                                            \
            if (installed())                                                                                           \
                return 0;                                                                                              \
            hookId = ::nmdk::HookRegistry::installPlt(MODULE, SYMBOL, ::nmdk::memory::toFuncPtr(&DEF_TYPE::detour),    \
                                                      static_cast<int>(PRIORITY), &DEF_TYPE::setOriginal, nullptr);    \
            return hookId ? 0 : -1;                                                                                    \
        }                                                                                                              \
        static bool unhook() {                                                                                         \
            if (!hookId)                                                                                               \
                return false;                                                                                          \
            if (!::nmdk::HookRegistry::remove(hookId))                                                                 \
                return false;                                                                                          \
            hookId = 0;                                                                                                \
            return true;                                                                                               \
        }                                                                                                              \
    };                                                                                                                 \
    REGISTER;                                                                                                          \
    RET_TYPE DEF_TYPE::detour(__VA_ARGS__)

#define NMDK_PLT_HOOK(DEF_TYPE, PRIORITY, SYMBOL, MODULE, RET_TYPE, ...)                                               \
    VA_EXPAND(NMDK_PLT_HOOK_IMPL(, DEF_TYPE, PRIORITY, SYMBOL, MODULE, RET_TYPE, __VA_ARGS__))

#define NMDK_AUTO_PLT_HOOK(DEF_TYPE, PRIORITY, SYMBOL, MODULE, RET_TYPE, ...)                                          \
    VA_EXPAND(NMDK_PLT_HOOK_IMPL(inline ::nmdk::memory::HookAutoRegister<DEF_TYPE> DEF_TYPE##AutoRegister, DEF_TYPE,   \
                                 PRIORITY, SYMBOL, MODULE, RET_TYPE, __VA_ARGS__))
