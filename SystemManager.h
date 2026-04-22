#pragma once
#include <set>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "EntityManager.h"
#include "Components.h"
#include "types.h"

namespace FrostEngine
{
    class ECS;
}

class ISystem
{
public:
    virtual ~ISystem() = default;
    virtual void Update() = 0;
    std::set<Entity> m_entities;
};

class Testsys : public ISystem
{   
public:
    FrostEngine::ECS* m_ecs;
    Testsys(FrostEngine::ECS *_ecs);
    void Update() override;
};

class SystemManager
{
public:
    template <typename T>
    void RegisterSystem(FrostEngine::ECS *_ecs)
    {
        auto index = typeid(T).hash_code();
        if (m_systems.find(index) != m_systems.end())
        {
            printf("System exists\n");
            return;
        }
        m_systems.emplace(index, new T{_ecs});
    }

    void EntitySignatureChange(Entity _entity, Signature &_signature)
    {
        for (auto &pair : m_systems)
        {
            auto &type = pair.first;
            auto &system = pair.second;
            auto &signature = m_systemSignature[type];

            if ((_signature & signature) == signature)
            {
                system->m_entities.insert(_entity);
            }
            else
            {
                system->m_entities.erase(_entity);
            }
        }
    }

    void EntityDestroyed(Entity _entity)
    {
        for (auto &pair : m_systems)
        {
            auto &system = pair.second;
            system->m_entities.erase(_entity);
        }
    }

    template<typename T>
    void SetSignature(Signature &_signature)
    {
        auto index = typeid(T).hash_code();
        if(m_systemSignature.find(index) != m_systemSignature.end())
        {
            m_systemSignature.at(index) = _signature;
        }
        else
        {
            printf("System not registered\n");
        }
    }
    
    std::unordered_map<size_t, ISystem*> Get() const
    {
        return m_systems;
    }

    std::unordered_map<size_t, ISystem*> m_systems;
    std::unordered_map<size_t, Signature> m_systemSignature;
};