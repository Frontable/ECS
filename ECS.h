#pragma once
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "Components.h"
#include "Logger/Logger.h"
#include "SpriteBatchRenderer.h"
#include <utility>

namespace FrostEngine
{
    class ECS
    {
    public:
        ECS()
        {
            m_componentManager.RegisterComponent<Transform2D>();
            m_componentManager.RegisterComponent<Velocity2D>();
            m_componentManager.RegisterComponent<Sprite>();
            m_componentManager.RegisterComponent<CircleCollider>();
            m_componentManager.RegisterComponent<Lifetime>();
            m_componentManager.RegisterComponent<PlayerTag>();
            m_componentManager.RegisterComponent<AsteroidTag>();
            m_componentManager.RegisterComponent<BulletTag>();
        }
        ECS(const ECS&) = delete;
        ECS& operator=(const ECS&) = delete;
        ~ECS() = default;

        //---------------------
        // Entity
        //---------------------
        Entity CreateEntity()
        {
            return m_entityManager.CreateEntity();
        }


        void DeleteEntity(Entity _entity)
        {
            m_componentManager.EntityDestroyed(_entity);
            m_entityManager.DeleteEntity(_entity);
        }

        //---------------------
        // Components
        //---------------------
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
            // FROST_LOG("aM I GETTING ERE");
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
        ComponentID GetComponentID()
        {
            return m_componentManager.GetComponentID<T>();
        }




        //---------------------
        // Systems
        //---------------------

        template <typename T, typename... Args>
        void RegisterSystem(Args &&...args)
        {
            m_systemManager.RegisterSystem<T>(std::forward<Args>(args)...);
        }

        template <typename T>
        T &getSystem()
        {
            return m_systemManager.getSystem<T>();
        }

        

        template <typename T>
        void SetSystemSignature(Signature _signature)
        {
            m_systemManager.SetSignature<T>(_signature);
        }

        enum class EventType
        {
            COLLISON = 0
        };

        struct EventMembers
        {
            EventMembers(Entity _triggered, Entity _trigger) : triggered(_triggered), trigger(_trigger) {}
            Entity triggered;
            Entity trigger;
        };

        void RegisterEvent(Entity _triggered, Entity _trigger)
        {
            m_eventMembers.emplace_back(_triggered, _trigger);
        }
        void fortest()
        {
            for (auto &pair : m_eventMembers)
            {
                auto &e1 = pair.triggered;
                auto &e2 = pair.trigger;
                // std::cout<<"Entity " << e1 << " collided with Entity" << e2 <<std::endl;
            }
            m_eventMembers.clear();
        }

        std::vector<EventMembers> &GetEvents()
        {
            return m_eventMembers;
        }

    private:
        EntityManager m_entityManager{};
        ComponentManager m_componentManager{};
        SystemManager m_systemManager;
        std::vector<EventMembers> m_eventMembers;
    };
}
