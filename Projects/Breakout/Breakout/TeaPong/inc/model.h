#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "mesh.h"

class Model
{
public:

   Model(std::vector<Mesh>&& meshes);

   Model(const Model&) = delete;
   Model& operator=(const Model&) = delete;

   Model(Model&& rhs) = default;
   Model& operator=(Model&& rhs) = default;

   void render(const Shader& shader) const;

private:

   std::vector<Mesh> mMeshes;
};

#endif
