#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "resource_manager.h"

class Model
{
public:

   Model(std::vector<std::unique_ptr<Mesh>>&& meshes, ResourceManager<Texture>&& texManager);

   Model(const Model&) = delete;
   Model& operator=(const Model&) = delete;

   Model(Model&& rhs) = default;
   Model& operator=(Model&& rhs) = default;

   void render(const Shader& shader) const;

private:

   std::vector<std::unique_ptr<Mesh>> mMeshes;
   ResourceManager<Texture>           mTexManager;
};

#endif
