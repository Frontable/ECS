#include "ECS.h"
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "draw.h"
#include "SystemManager.h"

// =======================
// SHADERS
// =======================
const char *v = R"(
#version 330 core
layout(location = 0) in vec2 vPos;

uniform mat4 projection;
uniform mat4 model;

void main()
{
    gl_Position = projection * model * vec4(vPos, 0.0, 1.0);
}
)";

const char *f = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.25, 1.0);
}
)";

// =======================
// CALLBACK
// =======================
static void glfwResize(GLFWwindow *win, int w, int h)
{
    glViewport(0, 0, w, h);
}

// =======================
// MAIN
// =======================

int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "test", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, glfwResize);

    // Load GL
    gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress));

    // IMPORTANT: set viewport once
    glViewport(0, 0, 800, 600);

    // =======================
    // GEOMETRY
    // =======================
    

    // =======================
    // SHADER
    // =======================
    Shader shader;
    shader.CreateFromSource(v, f);

    // =======================
    // GAME STATE
    // =======================
    float jumpVelocity = 0.0f;
    bool onGround = false;  // true if triangle is standing on "floor"
    float gravity = 1.5f;
    float jumpStrength = 20.0f; 
    //FrostEngine::ECS &ecs = FrostEngine::ECS::Get();

    // Top-left origin (Y goes down);


    FrostEngine::ECS &ecs = FrostEngine::ECS::Get();
    Entity e1 = ecs.CreateEntity();
    ecs.RegisterComponent<Transform2D>();
    ecs.RegisterComponent<RigidBody2D>();
    ecs.AddComponent(e1, Transform2D{vec2(100, 100), 0, vec2(100,100)});
    ecs.AddComponent(e1, RigidBody2D{vec2(5,5)});
    PhysicsSystem physicsSystem;
    RenderSystem renderSystem;
    physicsSystem.m_entities.emplace(e1);
    renderSystem.m_entities.emplace(e1);
    renderSystem.init();
    renderSystem.shader = &shader;
    
    auto &transform = ecs.GetComponent<Transform2D>(e1); 
    auto &rigid = ecs.GetComponent<RigidBody2D>(e1);
    // =======================
    // LOOP
    // =======================
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Movement
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        transform.position.x -= rigid.velocity.x;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        transform.position.x += rigid.velocity.x;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        transform.position.y -= rigid.velocity.y;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        transform.position.y += rigid.velocity.y;
        // On space press and on ground, start jump
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && onGround)
        {
            jumpVelocity = -jumpStrength; // negative because Y increases downward
            onGround = false;
        }

        // Apply jump velocity and gravity
        transform.position.y += jumpVelocity;   // move by current velocity
        jumpVelocity += gravity; // gravity accelerates downward

        // Check floor collision
        if (transform.position.y + transform.size.y * 0.5 >= 600) // bottom edge
        {
            transform.position.y = 600 - transform.size.y * 0.5f;
            jumpVelocity = 0;
            onGround = true;
        }

        // =======================
        // TRANSFORM
        // =======================



        //system
        physicsSystem.update(ecs);


        // =======================
        // RENDER
        // =======================
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderSystem.update(ecs);

        glfwSwapBuffers(window);
    }

    // =======================
    // CLEANUP
    // =======================
    //shader.Destroy();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
