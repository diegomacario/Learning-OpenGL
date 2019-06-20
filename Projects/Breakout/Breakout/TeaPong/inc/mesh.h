#ifndef MESH_H
#define MESH_H

#include <assimp/scene.h>

#include <vector>

#include "shader.h"
#include "texture.h"

struct Vertex
{
   glm::vec3 position;
   glm::vec3 normal;
   glm::vec2 texCoords;
};

struct MeshTexture
{
   MeshTexture(Texture&& tex, aiTextureType type, const std::string& filename)
      : tex(std::move(tex))
      , type(type)
      , filename(filename)
   {

   }

   MeshTexture(MeshTexture&) = delete;
   MeshTexture& operator=(MeshTexture&) = delete;

   MeshTexture(MeshTexture&& rhs)
      : tex(std::move(rhs.tex))
      , type(std::exchange(rhs.type, aiTextureType_NONE))
      , filename(std::move(filename)) // TODO: Does this leave the string in rhs empty?
   {

   }

   MeshTexture& operator=(MeshTexture&& rhs)
   {
      tex = std::move(rhs.tex);
      type = std::exchange(rhs.type, aiTextureType_NONE);
      filename = std::move(filename); // TODO: Does this leave the string in rhs empty?
      return *this;
   }

   Texture       tex;
   aiTextureType type;
   std::string   filename;
};

class Mesh
{
public:

   Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures);
   ~Mesh();

   Mesh(Mesh&) = delete;
   Mesh& operator=(Mesh&) = delete;

   Mesh(Mesh&& rhs);
   Mesh& operator=(Mesh&& rhs);

   void render(const Shader& shader) const;

private:

   GLsizei                   mNumIndices;
   std::vector<MeshTexture>  mTextures;
   GLuint                    mVAO;

   void configureVAO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
   void bindTextures(const Shader& shader) const;
};

#endif
