#ifndef MESH_H
#define MESH_H

#include <assimp/scene.h>

#include <vector>

#include "shader.h"

struct Vertex
{
   Vertex(const glm::vec3& position,
          const glm::vec3& normal,
          const glm::vec2& texCoords)
      : position(position)
      , normal(normal)
      , texCoords(texCoords)
   {

   }

   glm::vec3 position;
   glm::vec3 normal;
   glm::vec2 texCoords;
};

class Mesh
{
public:

   Mesh(const std::vector<Vertex>&       vertices,
        const std::vector<unsigned int>& indices);
   virtual ~Mesh();

   Mesh(const Mesh&) = delete;
   Mesh& operator=(const Mesh&) = delete;

   Mesh(Mesh&& rhs) noexcept;
   Mesh& operator=(Mesh&& rhs) noexcept;

   virtual void render(const Shader& shader) const = 0;

private:

   void configureVAO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

   GLsizei mNumIndices;
   GLuint  mVAO;
};

#endif
