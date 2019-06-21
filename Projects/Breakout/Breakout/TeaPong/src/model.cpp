#include "model.h"

Model::Model(std::vector<Mesh>& meshes)
   : mMeshes(std::move(meshes))
{

}

void Model::render(const Shader& shader) const
{
   for (auto &mesh : mMeshes)
   {
      mesh.render(shader);
   }
}
