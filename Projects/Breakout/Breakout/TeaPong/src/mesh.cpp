#include <iostream>

#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>&       vertices,
           const std::vector<unsigned int>& indices,
           const Material&                  material)
   : mNumIndices(indices.size())
   , mMaterial(material)
{
   configureVAO(vertices, indices);
}

Mesh::~Mesh()
{
   glDeleteVertexArrays(1, &mVAO);
}

Mesh::Mesh(Mesh&& rhs) noexcept
   : mNumIndices(std::exchange(rhs.mNumIndices, 0))
   , mMaterial(std::move(rhs.mMaterial)) // TODO: Is this move working?
   , mVAO(std::exchange(rhs.mVAO, 0))
{

}

Mesh& Mesh::operator=(Mesh&& rhs) noexcept
{
   mNumIndices = std::exchange(rhs.mNumIndices, 0);
   mMaterial   = std::move(rhs.mMaterial); // TODO: Is this move working?
   mVAO        = std::exchange(rhs.mVAO, 0);
   return *this;
}

void Mesh::render(const Shader& shader) const
{
   shader.use();

   // TODO: Set the uniforms here...
   bindMaterialTextures(shader);
   setMaterialTextureAvailabilities(shader);
   setMaterialConstants(shader);

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

   // TODO: Can I delete the VBO and EBO before unbinding the VAO? Is it necessary to unbind them before deleting them?
   glBindVertexArray(0);

   glDeleteBuffers(1, &VBO);
   glDeleteBuffers(1, &EBO);
}

void Mesh::bindMaterialTextures(const Shader& shader) const
{
   unsigned int texUnit = GL_TEXTURE0;

   for (unsigned int i = 0; i < mMaterial.textures.size(); ++i)
   {
      // Get the location of the sampler2D uniform that should exist in the shader
      GLint uniformLoc = glGetUniformLocation(shader.getID(), mMaterial.textures[i].uniformName.c_str());

      if (uniformLoc != -1)
      {
         // Activate the proper texture unit before binding the current texture
         glActiveTexture(texUnit);
         // Tell the sampler2D uniform in what texture unit to look for the texture data
         glUniform1i(uniformLoc, texUnit);
         // Bind the texture
         mMaterial.textures[i].texture->bind();

         ++texUnit;
      }
      else
      {
         std::cout << "Error - Mesh::bindTextures - The following sampler2D uniform does not exist: " << mMaterial.textures[i].uniformName << "\n";
      }
   }

   glActiveTexture(GL_TEXTURE0);
}

void Mesh::setMaterialTextureAvailabilities(const Shader& shader) const
{
   shader.setInt("materialTextureAvailabilities.ambientTexIsAvailable", mMaterial.textureAvailabilities.ambientTexIsAvailable ? 1 : 0);
   shader.setInt("materialTextureAvailabilities.emissiveTexIsAvailable", mMaterial.textureAvailabilities.emissiveTexIsAvailable ? 1 : 0);
   shader.setInt("materialTextureAvailabilities.diffuseTexIsAvailable", mMaterial.textureAvailabilities.diffuseTexIsAvailable ? 1 : 0);
   shader.setInt("materialTextureAvailabilities.specularTexIsAvailable", mMaterial.textureAvailabilities.specularTexIsAvailable ? 1 : 0);
}

void Mesh::setMaterialConstants(const Shader& shader) const
{
   shader.setVec3("materialConstants.ambient", mMaterial.constants.ambientColor);
   shader.setVec3("materialConstants.emissive", mMaterial.constants.emissiveColor);
   shader.setVec3("materialConstants.diffuse", mMaterial.constants.diffuseColor);
   shader.setVec3("materialConstants.specular", mMaterial.constants.specularColor);
   shader.setFloat("materialConstants.shininess", mMaterial.constants.shininess);
}
