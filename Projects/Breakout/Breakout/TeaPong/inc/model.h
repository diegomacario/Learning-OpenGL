#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>

#include <string>
#include <vector>
#include <map>

#include "mesh.h"
#include "shader.h"

class Model
{
public:

   Model(const std::vector<Mesh>& meshes);

   void render(const Shader& shader) const;

private:

   std::vector<Mesh> mMeshes;
};

#endif
