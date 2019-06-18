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
   MeshTexture(Texture tex, aiTextureType type, const std::string& filename)
      : tex(tex)
      , type(type)
      , filename(filename)
   {

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

   // TODO: Should it be possible to copy a mesh? Would that result in two textures pointing to the same memory in the GPU? Or two VAOs? What happens if glDeleteTextures or glDeleteArrays gets called twice?
   Mesh(Mesh&) = default;
   Mesh& operator=(Mesh&) = default;

   Mesh(Mesh&&) = default;
   Mesh& operator=(Mesh&&) = default;

   void render(const Shader& shader) const;

private:

   GLsizei                   mNumIndices;
   std::vector<MeshTexture>  mTextures;
   GLuint                    mVAO;

   void configureVAO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
   void bindTextures(const Shader& shader) const;
};

#endif
