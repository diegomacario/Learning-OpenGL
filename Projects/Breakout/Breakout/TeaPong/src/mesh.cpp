#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
{
   mVertices = vertices;
   mIndices  = indices;
   mTextures = textures;

   configureVAO();
}

void Mesh::draw(Shader shader) const
{
   // Bind the appropriate textures
   unsigned int diffuseNr  = 1;
   unsigned int specularNr = 1;
   unsigned int normalNr   = 1;
   unsigned int heightNr   = 1;

   for (unsigned int i = 0; i < mTextures.size(); ++i)
   {
      glActiveTexture(GL_TEXTURE0 + i); // Activate the proper texture unit before binding

      std::string number;
      std::string name = mTextures[i].type;
      if(name == "texture_diffuse")
         number = std::to_string(diffuseNr++);
      else if(name == "texture_specular")
         number = std::to_string(specularNr++);
      else if(name == "texture_normal")
         number = std::to_string(normalNr++);
      else if(name == "texture_height")
         number = std::to_string(heightNr++);

      // Set the sampler to the correct texture unit
      glUniform1i(glGetUniformLocation(shader.getID(), (name + number).c_str()), i);
      // Bind the texture
      glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
   }

   // Draw the mesh
   glBindVertexArray(mVAO);
   glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);

   // Always good practice to set everything back to default once configured
   glActiveTexture(GL_TEXTURE0);
}

void Mesh::configureVAO()
{
   GLuint VBO, EBO;

   glGenVertexArrays(1, &mVAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(mVAO);

   // Load data into the buffers
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW); // TODO: Is this okay?

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

   // Set the vertex attribute pointers

   // Vertex Positions
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
   // Vertex normals
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
   // Vertex texture coords
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

   glBindVertexArray(0);
}
