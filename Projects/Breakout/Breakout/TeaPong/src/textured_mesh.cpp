#include <iostream>

#include "textured_mesh.h"

TexturedMesh::TexturedMesh(const std::vector<Vertex>&          vertices,
                           const std::vector<unsigned int>&    indices,
                           const std::vector<MaterialTexture>& textures)
   : Mesh(vertices, indices)
   , mTextures(textures)
{

}

TexturedMesh::TexturedMesh(TexturedMesh&& rhs) noexcept
   : Mesh(std::move(rhs))
   , mTextures(std::move(rhs.mTextures)) // TODO: Is this move working?
{

}

TexturedMesh& TexturedMesh::operator=(TexturedMesh&& rhs) noexcept
{
   Mesh::operator=(std::move(rhs));
   mTextures = std::move(rhs.mTextures); // TODO: Is this move working?
   return *this;
}

void TexturedMesh::render(const Shader& shader) const
{
   shader.use();

   bindMaterialTextures(shader);

   // Draw the mesh
   Mesh::render(shader);
}

void TexturedMesh::bindMaterialTextures(const Shader& shader) const
{
   unsigned int texUnit = GL_TEXTURE0;

   for (unsigned int i = 0; i < mTextures.size(); ++i)
   {
      // Get the location of the sampler2D uniform that should exist in the shader
      GLint uniformLoc = glGetUniformLocation(shader.getID(), mTextures[i].uniformName.c_str());

      if (uniformLoc != -1)
      {
         // Activate the proper texture unit before binding the current texture
         glActiveTexture(texUnit);
         // Tell the sampler2D uniform in what texture unit to look for the texture data
         glUniform1i(uniformLoc, texUnit);
         // Bind the texture
         mTextures[i].texture->bind();

         ++texUnit;
      }
      else
      {
         std::cout << "Error - Mesh::bindTextures - The following sampler2D uniform does not exist: " << mTextures[i].uniformName << "\n";
      }
   }

   glActiveTexture(GL_TEXTURE0);
}
