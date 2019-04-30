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

   Model(const std::string& modelFilePath);
   // TODO: Add destructor that deletes the textures?

   void draw(const Shader& shader) const;

private:

   std::string                    mModelDir;
   std::vector<Mesh>              mMeshes;
   std::map<std::string, Texture> mLoadedTextures; // This map stores all the textures used by this model, so that if a texture is used by more than one mesh, it is only loaded once

   void loadModel(const std::string& modelFilePath);

   void processNodeHierarchyRecursively(const aiNode* node, const aiScene* scene);

   // These methods translate Assimp's data into the Mesh objects that form the model
   Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
   void processVertices(const aiMesh* mesh, std::vector<Vertex>& vertices);
   void processIndices(const aiMesh* mesh, std::vector<unsigned int>& indices);
   void processMaterial(const aiMaterial* material, std::vector<Texture>& textures);
   void processTextures(const aiMaterial* material, const aiTextureType texType, std::vector<Texture>& textures);
};

unsigned int loadTexture(const std::string& texFilename, const std::string& modelDir);

#endif
