#pragma once

#include "mc/_HeaderOutputPredefine.h"

#if NMDK_HAS_ENTT

#include "mc/deps/core/utility/optional_ref.h"
#include "mc/deps/ecs/EntityIdTraits.h"
#include "mc/deps/ecs/IEntityComponent.h"

template <> struct entt::entt_traits<EntityId> : entt::basic_entt_traits<EntityIdTraits> {
    static constexpr std::size_t page_size = 2048;
};

template <std::derived_from<IEntityComponent> Type> struct entt::component_traits<Type, EntityId> {
    using element_type = Type;
    using entity_type = EntityId;
    static constexpr bool in_place_delete = true;
    static constexpr std::size_t page_size = 128 * !std::is_empty_v<Type>;
};

template <typename Type> struct entt::storage_type<Type, EntityId> {
    using type = basic_storage<Type, EntityId>;
};

template <std::derived_from<IEntityComponent> Type> struct entt::type_hash<Type> {
    [[nodiscard]] static consteval id_type value() noexcept {
        constexpr auto hash = Type::type_hash;
        return hash;
    }

    [[nodiscard]] consteval operator id_type() const noexcept { return value(); }
};

class EntityRegistry : public std::enable_shared_from_this<EntityRegistry> {
  public:
    std::string name;
    entt::basic_registry<EntityId> ownedRegistry;
    uint id;

    template <typename T> T *tryGetGlobalComponent() { return this->ownedRegistry.ctx().find<T>(); }
};

class EntityContext {
  public:
    EntityRegistry &registry;
    entt::basic_registry<EntityId> &enttRegistry;
    EntityId entity;

  public:
    [[nodiscard]] inline entt::basic_registry<EntityId> &getRegistry() noexcept { return enttRegistry; }

    [[nodiscard]] inline entt::basic_registry<EntityId> const &getRegistry() const noexcept { return enttRegistry; }

    template <class T> [[nodiscard]] inline optional_ref<T const> tryGetComponent() const {
        return getRegistry().try_get<T>(entity);
    }

    template <class T> [[nodiscard]] inline optional_ref<T> tryGetComponent() {
        return const_cast<T *>(std::as_const(getRegistry()).try_get<T>(entity));
    }

    template <class T> [[nodiscard]] inline bool hasComponent() const { return getRegistry().all_of<T>(entity); }

    template <class T> inline bool removeComponent() { return getRegistry().remove<T>(entity); }

    template <class T, typename... Args> [[nodiscard]] inline T &getOrAddComponent(Args &&...args) {
        return getRegistry().get_or_emplace<T>(entity, std::forward<Args>(args)...);
    }
};

#else

struct EntityRegistry {};
struct EntityContext {};

#endif
