#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>

#include "model.h"

Model::Model(const std::string& modelFilePath)
{
   loadModel(modelFilePath);
}

void Model::render(const Shader& shader) const
{
   // TODO: Maybe we should call shader.use() here to help the user

   for (auto &mesh : mMeshes)
   {
      mesh.render(shader);
   }
}
