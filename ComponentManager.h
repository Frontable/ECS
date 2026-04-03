#pragma once
#include "EntityManager.h"
#include <unordered_map>
#include <memory>
#include <typeinfo>


namespace FrostEngine
{
    class IComponentArray
    {
        public:
        virtual ~IComponentArray() = default;
    };

    template<typename T>
    class ComponentArray : public IComponentArray
    {
        public:
        void AddComponent(Entity _entity, const T& _component)
        {
            m_Components.emplace(_entity, _component);
        }
        void RemoveComponent(Entity _entity);
        void SetComponent(Entity _entity, const T& _component);
        T& GetComponent(Entity _entity);

        private:
        std::unordered_map<Entity, T> m_Components;
    };

    class ComponentManager
    {
        public:
        std::unordered_map<size_t, IComponentArray*> m_componentArrays;

        template<typename T>
        bool RegisterComponent()
        {
            size_t type = typeid(T).hash_code();
            if(m_componentArrays.find(type) == m_componentArrays.end())
            {
                m_componentArrays.emplace(type, new ComponentArray<T>{});
                return true;
            }
            printf("Components array alreay exists\n");
            return false;    
        }

        template<typename T>
        void AddComponent(Entity _entity, T _component)
        {
            GetComponentArray<T>()->AddComponent(_entity, _component);
        }



        private:
        template<typename T>
        ComponentArray<T>* GetComponentArray()
        {
            size_t type = typeid(T).hash_code();
            return static_cast<ComponentArray<T>*>(m_componentArrays.at(type));
        }

    };
}