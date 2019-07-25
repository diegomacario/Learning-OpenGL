#include "model.h"

Model::Model(std::vector<std::unique_ptr<Mesh>>&& meshes, ResourceManager<Texture>&& texManager)
   : mMeshes(std::move(meshes))
   , mTexManager(std::move(texManager))
{

}

void Model::render(const Shader& shader) const
{
   for (auto &mesh : mMeshes)
   {
      mesh->render(shader);
   }
}
