#pragma once
#include <cstdint>
#include <bitset>
#include <queue>
#include <array>
#include <iostream>

using Entity = std::uint32_t;
using ComponentType = std::uint8_t;
constexpr ComponentType MAX_COMPONENTS = 32;
constexpr Entity MAX_ENTITIES = 5000;
using Signature = std::bitset<MAX_COMPONENTS>;

namespace FrostEngine
{
    class EntityManager
    {
    public:
        EntityManager() = default;
        // static constexpr Entity s_entity = 0;
        static EntityManager &Get()
        {
            static EntityManager instance;
            return instance;
        }

        Entity CreateEntity()
        {
            Entity entity;
            if (m_currentEntity >= MAX_ENTITIES)
            {
                throw std::runtime_error("Maximum numbers of entities reached");
            }
            if (!m_availableEntities.empty())
            {
                entity = m_availableEntities.front();
                m_availableEntities.pop();
                m_aliveEntities[entity] = true;
                m_entities[entity] = Signature();
                return entity;
            }
            entity = m_currentEntity++;
            m_aliveEntities[entity] = true;
            m_entities[entity] = Signature();
            std::cout << "Created entity with id:" << entity << std::endl;
            return entity;
        }

        void DeleteEntity(Entity _entity)
        {
            if (_entity >= MAX_ENTITIES)
            {
                std::cout << "Tried to delete an entity that is above the maximum number of entities" << std::endl;
                return;
            }
            if (m_aliveEntities[_entity])
            {
                std::cout << "Deleting entity with id:" << _entity << std::endl;
                m_aliveEntities[_entity] = false;
                m_entities[_entity] = Signature();
                m_availableEntities.push(_entity);
            }
        }

        void SetSignature(Entity _entity, Signature &signature)
        {
            if (m_aliveEntities[_entity] && _entity >= 0 && _entity <= MAX_ENTITIES)
            {
                m_entities[_entity] = signature;
                return;
            }
            std::cout << "Tryed to set a signature to Entity:" << _entity << "that does not exist" << std::endl;
            throw std::runtime_error("Tryed to get a signature from Entity that does not exist");
            return;
        }

        Signature GetSignature(Entity _entity) const
        {
            if (m_aliveEntities[_entity] && _entity >= 0 && _entity <= MAX_ENTITIES)
            {
                return m_entities[_entity];
            }
        }

    private:    
        Entity m_currentEntity{};
        std::array<Signature, MAX_ENTITIES> m_entities{};
        std::array<bool, MAX_ENTITIES> m_aliveEntities{};
        std::queue<Entity> m_availableEntities{};
    };
}
