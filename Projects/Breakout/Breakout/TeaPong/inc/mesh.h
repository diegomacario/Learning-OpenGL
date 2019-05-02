#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <assimp/scene.h>

#include <string>
#include <vector>

#include "shader.h"

struct Vertex
{
   glm::vec3 position;
   glm::vec3 normal;
   glm::vec2 texCoords;
};

struct Texture
{
   unsigned int  id;
   aiTextureType type;
   std::string   filename;
};

class Mesh
{
public:

   Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
   // TODO: Add destructor that deletes the VAO?

   void draw(const Shader& shader) const;

private:

   std::vector<Vertex>       mVertices;
   std::vector<unsigned int> mIndices;
   std::vector<Texture>      mTextures;

   GLuint mVAO;

   void configureVAO();
   void bindTextures(const Shader& shader) const;
};

#endif
