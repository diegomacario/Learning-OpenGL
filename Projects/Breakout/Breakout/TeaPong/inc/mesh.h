#ifndef MESH_H
#define MESH_H

#include <assimp/scene.h>

#include <memory>
#include <vector>
#include <bitset>

#include "shader.h"
#include "texture.h"

struct Vertex
{
   Vertex(const glm::vec3& position,
          const glm::vec3& normal,
          const glm::vec2& texCoords)
      : position(position)
      , normal(normal)
      , texCoords(texCoords)
   {

   }

   glm::vec3 position;
   glm::vec3 normal;
   glm::vec2 texCoords;
};

struct MaterialTexture
{
   MaterialTexture(const std::shared_ptr<Texture>& texture, const std::string& uniformName)
      : texture(texture)
      , uniformName(uniformName)
   {

   }

   std::shared_ptr<Texture> texture;
   std::string              uniformName;
};

enum class MaterialTextureTypes : unsigned int
{
   ambient  = 0,
   emissive = 1,
   diffuse  = 2,
   specular = 3,
   count    = 4
};

struct MaterialConstants
{
   MaterialConstants(const glm::vec3& ambientColor,
                     const glm::vec3& emissiveColor,
                     const glm::vec3& diffuseColor,
                     const glm::vec3& specularColor,
                     float            shininess)
      : ambientColor(ambientColor)
      , emissiveColor(emissiveColor)
      , diffuseColor(diffuseColor)
      , specularColor(specularColor)
      , shininess(shininess)
   {

   }

   glm::vec3 ambientColor;
   glm::vec3 emissiveColor;
   glm::vec3 diffuseColor;
   glm::vec3 specularColor;
   float     shininess;
};

struct Material
{
   Material::Material(const std::vector<MaterialTexture>&                                 materialTextures,
                      std::bitset<static_cast<unsigned int>(MaterialTextureTypes::count)> materialTextureAvailabilities,
                      const MaterialConstants&                                            materialConstants)
      : textures(materialTextures)
      , textureAvailabilities(materialTextureAvailabilities)
      , constants(materialConstants)
   {

   }

   std::vector<MaterialTexture>                                        textures;
   std::bitset<static_cast<unsigned int>(MaterialTextureTypes::count)> textureAvailabilities;
   MaterialConstants                                                   constants;
};

class Mesh
{
public:

   Mesh(const std::vector<Vertex>&       vertices,
        const std::vector<unsigned int>& indices,
        const Material&                  material);
   ~Mesh();

   Mesh(const Mesh&) = delete;
   Mesh& operator=(const Mesh&) = delete;

   Mesh(Mesh&& rhs) noexcept;
   Mesh& operator=(Mesh&& rhs) noexcept;

   void render(const Shader& shader) const;

private:

   void configureVAO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

   void bindMaterialTextures(const Shader& shader) const;
   void setMaterialTextureAvailabilities(const Shader& shader) const;
   void setMaterialConstants(const Shader& shader) const;

   GLsizei  mNumIndices;
   Material mMaterial;
   GLuint   mVAO;
};

#endif
