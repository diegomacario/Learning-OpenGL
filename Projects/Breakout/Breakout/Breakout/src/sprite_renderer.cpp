/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "sprite_renderer.h"


SpriteRenderer::SpriteRenderer(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D &texture,
                                glm::vec2 position,
                                glm::vec2 size,
                                GLfloat   rotate,
                                glm::vec3 color)
{
    // Prepare transformations
    this->shader.Use();
    glm::mat4 model;

    // Translate the quad to the right position
    // The position specifies the location of the top left corner
    model = glm::translate(model, glm::vec3(position, 0.0f));

    // Move the origin of rotation back to the top left corner
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    // Rotate the quad around its center
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    // Move origin of rotation to the center of the quad
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    // Scale the quad
    model = glm::scale(model, glm::vec3(size, 1.0f));

    // Render textured quad
    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
    GLfloat vertices[] = {
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f, // A
        1.0f, 0.0f, 1.0f, 0.0f, // B
        0.0f, 0.0f, 0.0f, 0.0f, // C

        0.0f, 1.0f, 0.0f, 1.0f, // D
        1.0f, 1.0f, 1.0f, 1.0f, // E
        1.0f, 0.0f, 1.0f, 0.0f  // F
    };

    /*
          When the origin is at the top left corner:
          ABC = Counterclockwise
          DEF = Counterclockwise

        (0,0)
          C----------BF
          |         / |
          |        /  |
          |       /   |
          |      /    |
          |     /     |
          |    /      |
          |   /       |
          |  /        |
          | /         |
          |/          |
          AD----------E
    */

    /*
          When the origin is at the bottom left corner:
          ABC = Clockwise
          DEF = Clockwise

          AD----------E
          | \         |
          |  \        |
          |   \       |
          |    \      |
          |     \     |
          |      \    |
          |       \   |
          |        \  |
          |         \ |
          |          \|
          C-----------BF
        (0,0)
    */

    // Configure VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure VAO
    glGenVertexArrays(1, &this->quadVAO);
    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}