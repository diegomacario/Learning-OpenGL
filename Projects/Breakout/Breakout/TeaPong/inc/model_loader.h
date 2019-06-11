#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <memory>

#include "model.h"

class ModelLoader
{
public:

   ModelLoader() = default;
   ~ModelLoader() = default; // TODO: Should the destructor delete the textures?

   ModelLoader(ModelLoader&) = delete;
   ModelLoader& operator=(ModelLoader&) = delete;

   ModelLoader(ModelLoader&&) = delete;
   ModelLoader& operator=(ModelLoader&&) = delete;

   std::shared_ptr<Model> loadResource(const std::string& modelFilePath) const;

private:

   std::string                    mModelDir;
   std::map<std::string, Texture> mLoadedTextures; // This map stores all the textures used by this model, so that if a texture is used by more than one mesh, it is only loaded once

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
