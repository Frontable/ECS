#pragma once
#include <cstdint>
#include <bitset>
#include <queue>
#include <array>
#include <iostream>
#include <stdexcept>

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

        static EntityManager& Get()
        {
            static EntityManager instance;
            return instance;
        }

        Entity CreateEntity()
        {
            Entity entity;
            if (m_currentEntity >= MAX_ENTITIES && m_availableEntities.empty())
            {
                throw std::runtime_error("Maximum number of entities reached");
            }

            if (!m_availableEntities.empty())
            {
                entity = m_availableEntities.front();
                m_availableEntities.pop();
            }
            else
            {
                entity = m_currentEntity++;
            }

            m_aliveEntities[entity] = true;
            m_entities[entity] = Signature();
            std::cout << "Created entity with id: " << entity << std::endl;
            return entity;
        }

        void DeleteEntity(Entity _entity)
        {
            if (_entity >= MAX_ENTITIES)
            {
                std::cout << "Tried to delete invalid entity\n";
                return;
            }

            if (m_aliveEntities[_entity])
            {
                std::cout << "Deleting entity with id: " << _entity << std::endl;
                m_aliveEntities[_entity] = false;
                m_entities[_entity] = Signature();
                m_availableEntities.push(_entity);
            }
        }

        void SetSignature(Entity _entity, const Signature& signature)
        {
            if (_entity < MAX_ENTITIES && m_aliveEntities[_entity])
            {
                m_entities[_entity] = signature;
                return;
            }

            throw std::runtime_error("Tried to set signature on invalid entity");
        }

        Signature GetSignature(Entity _entity) const
        {
            if (_entity < MAX_ENTITIES && m_aliveEntities[_entity])
            {
                return m_entities[_entity];
            }

            throw std::runtime_error("Tried to get signature from invalid entity");
        }

    private:
        Entity m_currentEntity{};
        std::array<Signature, MAX_ENTITIES> m_entities{};
        std::array<bool, MAX_ENTITIES> m_aliveEntities{};
        std::queue<Entity> m_availableEntities{};
    };
}