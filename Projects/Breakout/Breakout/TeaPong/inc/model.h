#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "mesh.h"

class Model
{
public:

   Model(const std::vector<Mesh>& meshes);

   void render(const Shader& shader) const;

private:

   std::vector<Mesh> mMeshes;
};

#endif
