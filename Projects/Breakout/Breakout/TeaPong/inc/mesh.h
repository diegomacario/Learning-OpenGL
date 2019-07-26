#ifndef MESH_H
#define MESH_H

#include <assimp/scene.h>

#include <memory>
#include <vector>

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

// TODO: Change the name of this struct to MaterialTexture
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

class Mesh
{
public:

   Mesh(const std::vector<Vertex>&          vertices,
        const std::vector<unsigned int>&    indices,
        const std::vector<MaterialTexture>& materialTextures,
        const MaterialConstants&            materialConstants);
   ~Mesh();

   Mesh(const Mesh&) = delete;
   Mesh& operator=(const Mesh&) = delete;

   Mesh(Mesh&& rhs) noexcept;
   Mesh& operator=(Mesh&& rhs) noexcept;

   void render(const Shader& shader) const;

private:

   void         configureVAO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

   void         bindMaterialTextures(const Shader& shader) const;

   void         setMaterialConstants(const Shader& shader) const;

   GLsizei                      mNumIndices;
   GLuint                       mVAO;

   std::vector<MaterialTexture> mMaterialTextures; // TODO: Shininess should be available to textured meshes too.

   MaterialConstants            mMaterialConstants;
};

#endif
