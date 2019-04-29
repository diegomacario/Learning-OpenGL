#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>

#include <vector>
#include <string>

#include "mesh.h"
#include "shader.h"

class Model
{
public:

   Model(const std::string& modelFilePath);

   void draw(const Shader& shader) const;

private:

   std::string          mModelFileDir;
   std::vector<Mesh>    mMeshes;
   std::vector<Texture> mLoadedTextures; // Stores all the textures loaded so far, so that each texture is not loaded more than once

   void loadModel(const std::string& modelFilePath);

   void processNodeHierarchyRecursively(const aiNode* node, const aiScene* scene);

   Mesh processMesh(const aiMesh* mesh, const aiScene* scene);

   void loadMaterialTextures(const aiMaterial* material, const aiTextureType texType, std::vector<Texture>& textures);
};

unsigned int TextureFromFile(const char* filename, const std::string& directory);

#endif
