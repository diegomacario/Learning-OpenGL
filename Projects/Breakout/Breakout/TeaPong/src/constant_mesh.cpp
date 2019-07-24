#include <iostream>

#include "constant_mesh.h"

ConstantMesh::ConstantMesh(const std::vector<Vertex>&       vertices,
                           const std::vector<unsigned int>& indices,
                           const MaterialConstants&         materialConstants)
   : Mesh(vertices, indices)
   , mMaterialConstants(materialConstants)
{

}

ConstantMesh::ConstantMesh(ConstantMesh&& rhs) noexcept
   : Mesh(std::move(rhs))
   , mMaterialConstants(std::move(rhs.mMaterialConstants)) // TODO: Is this move working?
{

}

ConstantMesh& ConstantMesh::operator=(ConstantMesh&& rhs) noexcept
{
   Mesh::operator=(std::move(rhs));
   mMaterialConstants = std::move(rhs.mMaterialConstants); // TODO: Is this move working?
   return *this;
}

void ConstantMesh::render(const Shader& shader) const
{
   shader.use();

   setMaterialConstants(shader);

   // Draw the mesh
   glBindVertexArray(mVAO);
   glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);
}

void ConstantMesh::setMaterialConstants(const Shader& shader) const
{
   shader.setVec3("materialConstants.ambient", mMaterialConstants.ambientColor);
   shader.setVec3("materialConstants.diffuse", mMaterialConstants.diffuseColor);
   shader.setVec3("materialConstants.specular", mMaterialConstants.specularColor);
   shader.setVec3("materialConstants.emissive", mMaterialConstants.emissiveColor);
   shader.setFloat("materialConstants.shininess", mMaterialConstants.shininess);
}
