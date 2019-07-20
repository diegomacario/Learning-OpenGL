#include <glm/glm.hpp>

#include <array>

#include "renderer_2D.h"

Renderer2D::Renderer2D(const std::shared_ptr<Shader>& shader)
   : mShader(shader)
{
   configureVAO();
}

Renderer2D::~Renderer2D()
{
   glDeleteVertexArrays(1, &mVAO);
}

Renderer2D::Renderer2D(Renderer2D&& rhs) noexcept
   : mShader(std::move(rhs.mShader))
   , mVAO(std::exchange(rhs.mVAO, 0))
{

}

Renderer2D& Renderer2D::operator=(Renderer2D&& rhs) noexcept
{
   mShader = std::move(rhs.mShader);
   mVAO    = std::exchange(rhs.mVAO, 0);
   return *this;
}

void Renderer2D::render(const GameObject2D& gameObj2D)
{
   mShader->use();
   mShader->setMat4("model", gameObj2D.getModelMatrix());

   glActiveTexture(GL_TEXTURE0);
   gameObj2D.getTexture()->bind();

   // Render textured quad
   glBindVertexArray(mVAO);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);
}

void Renderer2D::configureVAO()
{
   /*
      When the origin is at the top left corner:
      ABC = Counterclockwise
      ADB = Counterclockwise

      (0,0)
        C-----------B
        |         / |
        |        /  |
        |       /   |
        |      /    |
        |  1  /  2  |
        |    /      |
        |   /       |
        |  /        |
        | /         |
        |/          |
        A-----------D
   */

   /*
      When the origin is at the bottom left corner:
      ABC = Clockwise
      ADB = Clockwise

        A-----------D
        | \         |
        |  \        |
        |   \       |
        |    \      |
        |  1  \  2  |
        |      \    |
        |       \   |
        |        \  |
        |         \ |
        |          \|
        C-----------B
      (0,0)
   */

                                          // Pos      // Tex coords
   std::array<GLfloat, 24> vertices    = {0.0f, 1.0f, 0.0f, 1.0f,  // A
                                          1.0f, 0.0f, 1.0f, 0.0f,  // B
                                          0.0f, 0.0f, 0.0f, 0.0f,  // C
                                          1.0f, 1.0f, 1.0f, 1.0f}; // D

   std::array<unsigned int, 6> indices = {0, 1, 2,  // Triangle 1
                                          0, 3, 1}; // Triangle 2

   GLuint VBO, EBO;

   glGenVertexArrays(1, &mVAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(mVAO);

   // Load the quad's data into the buffers

   // Positions and texture coordinates
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
   // Indices
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

   // Set the vertex attribute pointers

   // Positions and texture coordinates
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);

   // TODO: Can I delete the VBO and EBO before unbinding the VAO? Is it necessary to unbind them before deleting them?
   glBindVertexArray(0);

   glDeleteBuffers(1, &VBO);
   glDeleteBuffers(1, &EBO);
}