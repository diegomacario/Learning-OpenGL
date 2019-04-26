#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "shader.h"

struct Vertex
{
   glm::vec3 Position;
   glm::vec3 Normal;
   glm::vec2 TexCoords;
   glm::vec3 Tangent;
   glm::vec3 Bitangent;
};

struct Texture
{
   unsigned int id;
   std::string  type;
   std::string  filename;
};

class Mesh
{
public:

   // Constructor
   Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

   // Render the mesh
   void draw(Shader shader);

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
