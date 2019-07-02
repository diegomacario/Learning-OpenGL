#ifndef MESH_H
#define MESH_H

#include <assimp/scene.h>

#include <vector>
#include <memory>

#include "shader.h"
#include "texture.h"

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

struct MeshTexture
{
   MeshTexture(const std::shared_ptr<Texture>& texture, const std::string& uniformName)
      : texture(texture)
      , uniformName(uniformName)
   {

   }

   std::shared_ptr<Texture> texture;
   std::string              uniformName;
};

class Mesh
{
public:

   Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures);
   ~Mesh();

   Mesh(const Mesh&) = delete;
   Mesh& operator=(const Mesh&) = delete;

   Mesh(Mesh&& rhs) noexcept;
   Mesh& operator=(Mesh&& rhs) noexcept;

   void render(const Shader& shader) const;

private:

   GLsizei                  mNumIndices;
   std::vector<MeshTexture> mTextures;
   GLuint                   mVAO;

   void configureVAO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
   void bindTextures(const Shader& shader) const;
};

#endif
