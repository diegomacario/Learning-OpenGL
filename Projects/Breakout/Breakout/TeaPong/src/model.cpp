#include "model.h"

Model::Model(std::vector<Mesh>&& meshes, ResourceManager<Texture>&& texManager)
   : mMeshes(std::move(meshes))
   , mTexManager(std::move(texManager))
{

}

void Model::render(const Shader& shader, bool useTextures) const
{
   for (auto &mesh : mMeshes)
   {
      mesh.render(shader, useTextures);
   }
}
