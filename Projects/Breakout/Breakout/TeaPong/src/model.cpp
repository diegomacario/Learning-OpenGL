#include "model.h"

Model::Model(const std::vector<Mesh>& meshes)
   : mMeshes(meshes)
{

}

void Model::render(const Shader& shader) const
{
   for (auto &mesh : mMeshes)
   {
      mesh.render(shader);
   }
}
