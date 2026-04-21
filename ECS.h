#pragma once
#include "EntityManager.h"
#include "ComponentManager.h"

namespace FrostEngine
{
    class ECS
    {
    public:
        static ECS& Get()
        {
            static ECS instance;
            return instance;
        }

        Entity CreateEntity()
        {
            return m_entityManager.CreateEntity();
        }

        void DeleteEntity(Entity _entity)
        {
            m_componentManager.EntityDestroyed(_entity);
            m_entityManager.DeleteEntity(_entity);
        }

        template<typename T>
        void RegisterComponent()
        {
            m_componentManager.RegisterComponent<T>();
        }

        template<typename T>
        void AddComponent(Entity _entity, T _component)
        {
            m_componentManager.AddComponent<T>(_entity, _component);

            ComponentID id = m_componentManager.GetComponentID<T>();
            Signature signature = m_entityManager.GetSignature(_entity);
            signature.set(id);
            m_entityManager.SetSignature(_entity, signature);
        }

        template<typename T>
        T& GetComponent(Entity _entity)
        {
            return m_componentManager.GetComponent<T>(_entity);
        }

    private:
        ECS() = default;

        EntityManager m_entityManager{};
        ComponentManager m_componentManager{};
    };
}