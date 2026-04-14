#pragma once
#include "ECS/ECS.h"
#include "ECS/EntityManager.h"
#include "ECS/Components.h"
#include <set>
#include "ECS/draw.h"


class System
{
    public:    
    //System() = default;
    virtual ~System() = default;
    std::set<Entity> m_entities;
    virtual void update(FrostEngine::ECS &ecs) = 0;
};

class PhysicsSystem : public System
{

    public:
    PhysicsSystem() = default;
    //test physics system
    // will get each entity transform and rigit body comp and calculate with them
    void update(FrostEngine::ECS &ecs) override
    {
        for(auto &e : m_entities)
        {
            auto &transform = ecs.GetComponent<Transform2D>(e);
            auto &rigid = ecs.GetComponent<RigidBody2D>(e);



            //std::cout<<transform.position.x <<std::endl;
        }
    }

};

class RenderSystem : public System
{
public:
    Draw *drawer;
    Shader *shader;
    mat4 projection;

    void init()
    {
        projection = ortho(0.0f, 800.0f, 600.0f, 0.0f);
        //printf("HI\n");
    }

    void update(FrostEngine::ECS &ecs) override
    {
        shader->Bind();

        // Set projection ONCE per frame
        shader->SetMatrix4("projection", projection);

        for (auto &e : m_entities)
        {
            auto &transform = ecs.GetComponent<Transform2D>(e);

            // Get model matrix from component
            mat4 model = transform.GetModelMatrix();

            shader->SetMatrix4("model", model);

            drawer->draw();
        }
    }
};

class ScriptingSystem
{
    public:
    ScriptingSystem(ECS &_ecs)
    :m_ecs{_ecs}, m_MainLoaded{false}
    {

    }
    ~ScriptingSystem() = default;
    bool LoadMainScript(sol::state &lua)
    {


        return false;
    }

    private:
    ECS& m_ecs;
    bool m_MainLoaded;

    void update();
    void render();
}