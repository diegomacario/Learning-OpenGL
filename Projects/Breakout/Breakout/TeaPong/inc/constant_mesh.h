#ifndef CONSTANT_MESH_H
#define CONSTANT_MESH_H

#include <assimp/scene.h>

#include "mesh.h"

struct MaterialConstants
{
   MaterialConstants(const glm::vec3& ambientColor,
                     const glm::vec3& diffuseColor,
                     const glm::vec3& specularColor,
                     const glm::vec3& emissiveColor,
                     float            shininess)
      : ambientColor(ambientColor)
      , diffuseColor(diffuseColor)
      , specularColor(specularColor)
      , emissiveColor(emissiveColor)
      , shininess(shininess)
   {

   }

   glm::vec3 ambientColor;
   glm::vec3 diffuseColor;
   glm::vec3 specularColor;
   glm::vec3 emissiveColor;
   float     shininess;
};

class ConstantMesh : public Mesh
{
public:

   ConstantMesh(const std::vector<Vertex>&       vertices,
                const std::vector<unsigned int>& indices,
                const MaterialConstants&         materialConstants);
   ~ConstantMesh() = default;

   ConstantMesh(const ConstantMesh&) = delete;
   ConstantMesh& operator=(const ConstantMesh&) = delete;

   // TODO: These move assignment operators could be made default.
   ConstantMesh(ConstantMesh&& rhs) noexcept;
   ConstantMesh& operator=(ConstantMesh&& rhs) noexcept;

   void render(const Shader& shader) const override;

private:

   void setMaterialConstants(const Shader& shader) const;

   MaterialConstants mMaterialConstants;
};

#endif
