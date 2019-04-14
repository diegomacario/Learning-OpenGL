#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // Holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

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
   string       type;
   string       filename;
};

class Mesh
{
public:
   // Mesh Data
   vector<Vertex>       vertices;
   vector<unsigned int> indices;
   vector<Texture>      textures;
   unsigned int         VAO;

   // Constructor
   Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
   {
      this->vertices = vertices;
      this->indices  = indices;
      this->textures = textures;

      // Now that we have all the required data, set the vertex buffers
      // and their attribute pointers
      setupMesh();
   }

   // Render the mesh
   void Draw(Shader shader)
   {
      // Bind the appropriate textures
      unsigned int diffuseNr  = 1;
      unsigned int specularNr = 1;
      unsigned int normalNr   = 1;
      unsigned int heightNr   = 1;

      for (unsigned int i = 0; i < textures.size(); i++)
      {
         glActiveTexture(GL_TEXTURE0 + i); // Activate the proper texture unit before binding

         string number;
         string name = textures[i].type;
         if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
         else if(name == "texture_specular")
            number = std::to_string(specularNr++);
         else if(name == "texture_normal")
            number = std::to_string(normalNr++);
         else if(name == "texture_height")
            number = std::to_string(heightNr++);

         // Set the sampler to the correct texture unit
         glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
         // Bind the texture
         glBindTexture(GL_TEXTURE_2D, textures[i].id);
      }

      // Draw the mesh
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);

      // Always good practice to set everything back to default once configured
      glActiveTexture(GL_TEXTURE0);
   }

private:
   // Render data
   unsigned int VBO, EBO;

   // Initializes all the buffer objects/arrays
   void setupMesh()
   {
      // Create buffers/arrays
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);
      // Load data into vertex buffers
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      // A great thing about structs is that their memory layout is sequential for all their items
      // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
      // again translates to 3/2 floats which translates to a byte array
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

      // Set the vertex attribute pointers
      // Vertex Positions
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
      // Vertex normals
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
      // Vertex texture coords
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
      // Vertex tangent
      glEnableVertexAttribArray(3);
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
      // Vertex bitangent
      glEnableVertexAttribArray(4);
      glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

      glBindVertexArray(0);
   }
};

#endif
