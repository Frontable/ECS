#include "SystemManager.h"
#include "ECS.h"  // Now we can include the full ECS definition

Testsys::Testsys(FrostEngine::ECS* _ecs) : m_ecs{_ecs} {}

void Testsys::Update()
{
    for(auto &entity : m_entities)
    {
        auto &trans = m_ecs->GetComponent<Transform2D>(entity);
        std::cout << "Entity with id " << entity << " has transform component at position (" 
                  << trans.position.x << ", " << trans.position.y << ")" << std::endl;
    }
}