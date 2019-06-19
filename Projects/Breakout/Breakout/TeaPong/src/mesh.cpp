#include <iostream>

#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures)
   : mNumIndices(indices.size())
   , mTextures(textures)
{
   configureVAO(vertices, indices);
}

Mesh::~Mesh()
{
   glDeleteVertexArrays(1, &mVAO);
}

Mesh::Mesh(Mesh&& rhs)
   : mNumIndices(std::exchange(rhs.mNumIndices, 0))
   , mTextures(std::move(rhs.mTextures))
   , mVAO(std::exchange(rhs.mVAO, 0))
{

}

Mesh& Mesh::operator=(Mesh&& rhs)
{
   mNumIndices = std::exchange(rhs.mNumIndices, 0);
   mTextures   = std::move(rhs.mTextures);
   mVAO        = std::exchange(rhs.mVAO, 0);
   return *this;
}

void Mesh::render(const Shader& shader) const
{
   // Bind the textures
   bindTextures(shader);

   // Draw the mesh
   glBindVertexArray(mVAO);
   glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);
}

void Mesh::configureVAO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
   GLuint VBO, EBO;

   glGenVertexArrays(1, &mVAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(mVAO);

   // Load the mesh's data into the buffers

   // Positions, normals and texture coordinates
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
   // Indices
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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

void Mesh::bindTextures(const Shader& shader) const
{
   unsigned int ambientNum  = 0;
   unsigned int emissiveNum = 0;
   unsigned int diffuseNum  = 0;
   unsigned int specularNum = 0;
   unsigned int texUnit     = GL_TEXTURE0;

   for (unsigned int i = 0; i < mTextures.size(); ++i)
   {
      // Compose the name of the sampler2D uniform that should exist in the shader
      std::string sampler2DUniformName;
      switch (mTextures[i].type)
      {
         // TODO: Would it be a good idea to somehow incorporate the filename of texture into our naming convention?
         case aiTextureType_AMBIENT:
            sampler2DUniformName = "ambientTex" + ambientNum;
            ++ambientNum;
            break;
         case aiTextureType_EMISSIVE:
            sampler2DUniformName = "emissiveTex" + emissiveNum;
            ++emissiveNum;
            break;
         case aiTextureType_DIFFUSE:
            sampler2DUniformName = "diffuseTex" + diffuseNum;
            ++diffuseNum;
            break;
         case aiTextureType_SPECULAR:
            sampler2DUniformName = "specularTex" + specularNum;
            ++specularNum;
            break;
         default:
            std::cout << "Error - Mesh::bindTextures - The following texture is of an invalid type (" << mTextures[i].type << "): " << mTextures[i].filename << "\n";
            sampler2DUniformName = "";
            break;
      }

      // Get the location of the sampler2D uniform that should exist in the shader
      GLint sampler2DUniformLoc = glGetUniformLocation(shader.getID(), sampler2DUniformName.c_str());

      if (sampler2DUniformLoc != -1)
      {
         // Activate the proper texture unit before binding the current texture
         glActiveTexture(texUnit);
         // Tell the sampler2D uniform in what texture unit to look for the texture data
         glUniform1i(sampler2DUniformLoc, texUnit);
         // Bind the texture
         mTextures[i].tex.Bind();

         ++texUnit;
      }
      else
      {
         std::cout << "Error - Mesh::bindTextures - The following sampler2D uniform does not exist: " << sampler2DUniformName << "\n";
      }
   }

   glActiveTexture(GL_TEXTURE0);
}
