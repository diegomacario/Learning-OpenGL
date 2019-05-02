#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

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

   unsigned int ambientNum  = 0;
   unsigned int emissiveNum = 0;
   unsigned int diffuseNum  = 0;
   unsigned int specularNum = 0;

   for (unsigned int i = 0; i < mTextures.size(); ++i)
   {
      // Activate the proper texture unit before binding the current texture
      glActiveTexture(GL_TEXTURE0 + i);

      // Compose the name of the sampler2D uniform that should exist in the shader
      std::string name;
      switch (mTextures[i].type)
      {
         case aiTextureType_AMBIENT:
            name = "ambientTex" + ambientNum;
            ++ambientNum;
            break;
         case aiTextureType_EMISSIVE:
            name = "emissiveTex" + emissiveNum;
            ++emissiveNum;
            break;
         case aiTextureType_DIFFUSE:
            name = "diffuseTex" + diffuseNum;
            ++diffuseNum;
            break;
         case aiTextureType_SPECULAR:
            name = "specularTex" + specularNum;
            ++specularNum;
            break;
         default:
            std::cout << "Error - The following texture has an invalid type (" << mTextures[i].type << "): " << mTextures[i].filename << "\n";
            name = "";
            break;
      }

      // Get the location of the sampler2D uniform that should exist in the shader
      GLint samplerUniformLoc = glGetUniformLocation(shader.getID(), name.c_str());

      if (samplerUniformLoc != -1)
      {
         // Tell the sampler2D uniform in what texture unit to look for the texture data
         glUniform1i(samplerUniformLoc, i);
      }
      else
      {
         std::cout << "Error - The following sampler2D uniform does not exist: " << name << "\n";
      }

      // Bind the texture
      glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
   }

   // Draw the mesh
   glBindVertexArray(mVAO);
   glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);

   glActiveTexture(GL_TEXTURE0);
}

void Mesh::configureVAO()
{
   GLuint VBO, EBO;

   glGenVertexArrays(1, &mVAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(mVAO);

   // Load the mesh's data into the buffers

   // Positions, normals and texture coordinates
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
   // Indices
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

   // Set the vertex attribute pointers

   // Positions
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
   // Normals
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
   // Texture coords
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

   glBindVertexArray(0);

   glDeleteBuffers(1, &VBO);
   glDeleteBuffers(1, &EBO);
}
