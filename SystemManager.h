#pragma once
#include <vector>
#include "algorithm"
#include <unordered_map>
#include <typeinfo>
#include "types.h"
#include "Components.h"
#include "EntityManager.h"
#include "Logger/Logger.h"



class ISystem
{
public:
    virtual ~ISystem() = default;
    virtual void Update() = 0;
    std::vector<Entity> m_entities;
};

class SystemManager
{
public:
    template <typename T>
    void RegisterSystem()
    {
        auto index = typeid(T).hash_code();
        if (m_systems.find(index) == m_systems.end())
        {
            m_systems.emplace(index, std::make_unique<T>());
            m_systemSignature.emplace(index, Signature{});
            FROST_LOG("Registered system");
        }
        else
        {
            FROST_LOG("System is already registered");
        }
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
                auto it = std::find(system->m_entities.begin(), system->m_entities.end(), _entity);
                if(it == system->m_entities.end())
                {
                    system->m_entities.emplace_back(_entity);
                }
                
            }
            else
            {
                auto it = std::find(system->m_entities.begin(), system->m_entities.end(), _entity);
                if(it != system->m_entities.end())
                {
                    //system->m_entities.swap()
                    std::iter_swap(it, system->m_entities.end() - 1);
                    system->m_entities.pop_back();

                }
                
            }
        }
    }

    void EntityDestroyed(Entity _entity)
    {
        for (auto &pair : m_systems)
        {
            auto &system = pair.second;
            auto it = std::find(system->m_entities.begin(), system->m_entities.end(), _entity);
                if(it != system->m_entities.end())
                {
                    //system->m_entities.swap()
                    std::iter_swap(it, system->m_entities.end() - 1);
                    system->m_entities.pop_back();

                }
        }
    }

    template <typename T>
    void SetSignature(Signature &_signature)
    {
        auto index = typeid(T).hash_code();

        if (m_systems.find(index) != m_systems.end())
        {
            m_systemSignature.at(index) = _signature;
        }
        else
        {
            FROST_LOG("System is not registered");
        }
    }

    template<typename T>
    T& getSystem()
    {
        auto index = typeid(T).hash_code();
        return *static_cast<T*>(m_systems.at(index).get());
    }

    std::unordered_map<size_t, std::unique_ptr<ISystem>> m_systems;
    std::unordered_map<size_t, Signature> m_systemSignature;
};

// class SystemManager
// {
// public:
//     template <typename T>
//     void RegisterSystem()
//     {
//         auto index = typeid(T).hash_code();
//         if (m_systems.find(index) == m_systems.end())
//         {
//             m_systems.emplace(index, std::make_unique<T>());
//             m_systemSignature.emplace(index, Signature{});
//             FROST_LOG("Registered system");
//         }
//         else
//         {
//             FROST_LOG("System is already registered");
//         }
//     }

//     void EntitySignatureChange(Entity _entity, Signature &_signature)
//     {
//         for (auto &pair : m_systems)
//         {
//             auto type = pair.first;
//             auto &system = pair.second;

//             auto it = m_systemSignature.find(type);
//             if (it == m_systemSignature.end())
//             {
//                 FROST_LOG("Missing system signature");
//                 continue;
//             }

//             auto &signature = it->second;

//             if ((_signature & signature) == signature)
//                 system->m_entities.insert(_entity);
//             else
//                 system->m_entities.erase(_entity);
//         }
//     }

//     void EntityDestroyed(Entity _entity)
//     {
//         for (auto &pair : m_systems)
//             pair.second->m_entities.erase(_entity);
//     }

//     template <typename T>
//     void SetSignature(Signature &_signature)
//     {
//         auto index = typeid(T).hash_code();
//         if (m_systems.find(index) != m_systems.end())
//         {
//             m_systemSignature[index] = _signature;
//         }
//         else
//         {
//             FROST_LOG("System is not registered");
//         }
//     }

//     template <typename T>
//     T& getSystem()
//     {
//         auto index = typeid(T).hash_code();
//         return *static_cast<T*>(m_systems.at(index).get());
//     }

//     std::unordered_map<size_t, std::unique_ptr<ISystem>> m_systems;
//     std::unordered_map<size_t, Signature> m_systemSignature;
// };