/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "text_renderer.h"
#include "resource_manager.h"


TextRenderer::TextRenderer(GLuint width, GLuint height)
{
    // Load and configure the shader
    this->TextShader = ResourceManager::LoadShader("shaders/text.vs", "shaders/text.frag", nullptr, "text");

    this->TextShader.SetMatrix4("projection",
                                glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f),
                                GL_TRUE);

    this->TextShader.SetInteger("text", 0);

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // 6 vertices of 4 floats (2 for coords and 2 for tex coords)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::Load(std::string font, GLuint fontSize)
{
    // Clear the previously loaded Characters
    this->Characters.clear();

    // Initialize the FreeType library
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Disable the byte-alignment restriction and load/compile/store the first 128 ASCII characters of the font
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Generate texture (8-bit grayscale image)
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       
        // Now store character for later use
        Character character = { texture,
                                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                                face->glyph->advance.x };

        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Configure the shader
    this->TextShader.Use();
    this->TextShader.SetVector3f("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    // Iterate through all the characters of the string
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        // See Text Rendering tutorial to understand these calculations
        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO with the appropriate dimensions for each character
        // The vertices are specified in ccwise order
        // The texture coordinates are inverted vertically (origin at top left instead of bottom left)
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 1.0 }, // Bottom left
            { xpos + w, ypos,       1.0, 0.0 }, // Top Right
            { xpos,     ypos,       0.0, 0.0 }, // Top left

            { xpos,     ypos + h,   0.0, 1.0 }, // Bottom left
            { xpos + w, ypos + h,   1.0, 1.0 }, // Bottom right
            { xpos + w, ypos,       1.0, 0.0 }  // Top right
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Now advance cursors for next glyph (note that advance is specified in 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
