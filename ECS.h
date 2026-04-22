#pragma once
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "Components.h"

namespace FrostEngine
{
    class ECS
    {
    public:
        ECS()
        {
            m_componentManager.RegisterComponent<Transform2D>();
            m_componentManager.RegisterComponent<ScriptComponent>();
            m_componentManager.RegisterComponent<RigidBody2D>();
            m_systemManager.RegisterSystem<Testsys>(this);

            Signature testSignature;
            testSignature.set(m_componentManager.GetComponentID<Transform2D>());
            testSignature.set(m_componentManager.GetComponentID<RigidBody2D>());
            m_systemManager.SetSignature<Testsys>(testSignature);

        }

        ~ECS() = default;

        Entity CreateEntity()
        {
            return m_entityManager.CreateEntity();
        }

        void DeleteEntity(Entity _entity)
        {
            m_componentManager.EntityDestroyed(_entity);
            m_entityManager.DeleteEntity(_entity);
        }

        template <typename T>
        void RegisterComponent()
        {
            m_componentManager.RegisterComponent<T>();
        }

        template <typename T>
        void AddComponent(Entity _entity, T _component)
        {
            m_componentManager.AddComponent<T>(_entity, _component);

            ComponentID id = m_componentManager.GetComponentID<T>();
            Signature signature = m_entityManager.GetSignature(_entity);
            signature.set(id);
            m_entityManager.SetSignature(_entity, signature);

            m_systemManager.EntitySignatureChange(_entity, signature);
        }

        template <typename T>
        void RemoveComponent(Entity _entity, T _component)
        {
            m_componentManager.RemoveComponent(_entity, _component);
        }

        template <typename T>
        T &GetComponent(Entity _entity)
        {
            return m_componentManager.GetComponent<T>(_entity);
        }

        template <typename T>
        void RegisterSystem()
        {
            m_systemManager.RegisterSystem<T>();
        }

        void testUpdate()
        {
            std::unordered_map<size_t, ISystem*> m_systems = m_systemManager.Get();

            for(auto pair : m_systems)
            {
                pair.second->Update();
            }

        }
    private:
        EntityManager m_entityManager{};
        ComponentManager m_componentManager{};
        SystemManager m_systemManager{};
    };
}