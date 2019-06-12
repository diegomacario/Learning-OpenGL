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

   mutable std::string                    mModelDir;
   mutable std::map<std::string, Texture> mLoadedTextures; // This map stores all the textures used by this model, so that if a texture is used by more than one mesh, it is only loaded once

   void processNodeHierarchyRecursively(const aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes) const;

   // These methods translate Assimp's data into the Mesh objects that form the model
   Mesh processMesh(const aiMesh* mesh, const aiScene* scene) const;
   void processVertices(const aiMesh* mesh, std::vector<Vertex>& vertices) const;
   void processIndices(const aiMesh* mesh, std::vector<unsigned int>& indices) const;
   void processMaterial(const aiMaterial* material, std::vector<Texture>& textures) const;
   void processTextures(const aiMaterial* material, const aiTextureType texType, std::vector<Texture>& textures) const;
};

unsigned int loadTexture(const std::string& texFilePath);

#endif
