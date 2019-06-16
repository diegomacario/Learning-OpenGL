#include "model.h"

Model::Model(const std::vector<Mesh>& meshes)
   : mMeshes(meshes)
{

}

void Model::render(const Shader& shader) const
{
   // TODO: Maybe we should call shader.use() here to help the user
   for (auto &mesh : mMeshes)
   {
      mesh.render(shader);
   }
}
