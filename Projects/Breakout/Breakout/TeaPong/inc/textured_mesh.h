#ifndef TEXTURED_MESH_H
#define TEXTURED_MESH_H

#include <assimp/scene.h>

#include <memory>

#include "texture.h"
#include "mesh.h"

// TODO: Change the name of this struct to MaterialTexture
struct MeshTexture
{
   MeshTexture(const std::shared_ptr<Texture>& texture, const std::string& uniformName)
      : texture(texture)
      , uniformName(uniformName)
   {

   }

   std::shared_ptr<Texture> texture;
   std::string              uniformName;
};

class TexturedMesh : public Mesh
{
public:

   TexturedMesh(const std::vector<Vertex>&       vertices,
                const std::vector<unsigned int>& indices,
                const std::vector<MeshTexture>&  textures);
   ~TexturedMesh() = default;

   TexturedMesh(const TexturedMesh&) = delete;
   TexturedMesh& operator=(const TexturedMesh&) = delete;

   // TODO: These move assignment operators could be made default.
   TexturedMesh(TexturedMesh&& rhs) noexcept;
   TexturedMesh& operator=(TexturedMesh&& rhs) noexcept;

   void render(const Shader& shader) const override;

private:

   void bindMaterialTextures(const Shader& shader) const;

   std::vector<MeshTexture> mTextures; // TODO: Shininess should be available to textured meshes too.
};

#endif
