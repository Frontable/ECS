#pragma once
#include "EntityManager.h"
#include <unordered_map>
#include <memory>
#include <typeinfo>

using ComponentID = std::uint8_t;
namespace FrostEngine
{
    class IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(Entity _entity) = 0;
    };

    template <typename T>
    class ComponentArray : public IComponentArray
    {
    public:

        // bool Has(Entity _entity)
        // {
        //     return m_Components.contains(_entity);
        // }
        void AddComponent(Entity _entity, const T &_component)
        {
            if(m_Components.find(_entity) == m_Components.end())
                m_Components.emplace(_entity, _component);
            else
                printf("Entity already has this component\n");
        }

        void RemoveComponent(Entity _entity)
        {
            if(m_Components.find(_entity) != m_Components.end())
                m_Components.erase(_entity);        
            else
                printf("Entity does not hat this comp\n");

        }

        void EntityDestroyed(Entity _entity) override
        {
            RemoveComponent(_entity);
        }

        void SetComponent(Entity _entity, const T &_component)
        {
            m_Components[_entity] = _component;
        }

        T &GetComponent(Entity _entity)
        {
            return m_Components[_entity];
        }

    private:
        std::unordered_map<Entity, T> m_Components;
    };

    class ComponentManager
    {
    public:
        std::unordered_map<size_t, IComponentArray *> m_componentArrays;

        template <typename T>
        bool RegisterComponent()
        {
            size_t type = typeid(T).hash_code();
            if (m_componentArrays.find(type) == m_componentArrays.end())
            {
                m_componentID.emplace(type, currentID++);
                m_componentArrays.emplace(type, new ComponentArray<T>{});
                return true;
            }
            printf("Components array alreay exists\n");
            return false;
        }

        template <typename T>
        void AddComponent(Entity _entity, T _component)
        {
            GetComponentArray<T>()->AddComponent(_entity, _component);
        }

        template <typename T>
        void RemoveComponent(Entity _entity)
        {
            GetComponentArray<T>()->RemoveComponent(_entity);
        }

        void EntityDestroyed(Entity _entity)
        {
            for (auto &map : m_componentArrays)
            {
                auto &component = map.second;
                component->EntityDestroyed(_entity);
            }
        }

        template <typename T>
        ComponentID GetComponentID()
        {
            size_t type = typeid(T).hash_code();
            return m_componentID.at(type);
        }

        template<typename T>
        T& GetComponent(Entity _entity)
        {
            return GetComponentArray<T>()->GetComponent(_entity);
        }

    private:
        template <typename T>
        ComponentArray<T> *GetComponentArray()
        {
            size_t type = typeid(T).hash_code();
            return static_cast<ComponentArray<T> *>(m_componentArrays.at(type));
        }

        std::unordered_map<size_t, ComponentID> m_componentID;
        ComponentID currentID{};
    };
}