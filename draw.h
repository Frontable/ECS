#pragma once
#include "Math.h"
#include "glad/glad.h"
#include "Shader.h"


class Draw
{
public:
    unsigned int vao, vbo;

    const float vertices[12] =
    {
        -0.5f,-0.5f,
         0.5f,-0.5f,
         0.5f, 0.5f,
         0.5f, 0.5f,
        -0.5f, 0.5f,
        -0.5f,-0.5f,
    };

    Draw() { init(); }

    ~Draw()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    void init()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    void draw()
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
};
