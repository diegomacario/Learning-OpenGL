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

   // Constructor
   Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

   // Render the mesh
   void draw(Shader shader) const;

private:

   // Mesh Data
   std::vector<Vertex>       vertices;
   std::vector<unsigned int> indices;
   std::vector<Texture>      textures;

   // Render data
   unsigned int VAO, VBO, EBO;

   // Initializes all the buffer objects/arrays
   void setupMesh();
};

#endif
