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
   // TODO: Add destructor that deletes the VAO and the textures?

   void render(const Shader& shader) const;

private:

   GLsizei                   mNumIndices;
   std::vector<MeshTexture>  mTextures;
   GLuint                    mVAO;

   void configureVAO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
   void bindTextures(const Shader& shader) const;
};

#endif
