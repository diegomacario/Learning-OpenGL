#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <unordered_map>

#include "model.h"
#include "resource_manager.h"

class ModelLoader
{
public:

   ModelLoader() = default;
   ~ModelLoader() = default; // TODO: Should the destructor delete the textures?

   ModelLoader(const ModelLoader&) = delete;
   ModelLoader& operator=(const ModelLoader&) = delete;

   ModelLoader(ModelLoader&&) = delete;
   ModelLoader& operator=(ModelLoader&&) = delete;

   Model loadResource(const std::string& modelFilePath) const;

private:

   mutable std::string mModelDir;

   void processNodeHierarchyRecursively(const aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes, ResourceManager<Texture>& texManager) const;

   // These methods translate Assimp's data into the Mesh objects that form the model
   Mesh processMesh(const aiMesh* mesh, const aiScene* scene, ResourceManager<Texture>& texManager) const;
   std::vector<Vertex>       processVertices(const aiMesh* mesh) const;
   std::vector<unsigned int> processIndices(const aiMesh* mesh) const;
   std::vector<MeshTexture>  processMaterial(const aiMaterial* material, ResourceManager<Texture>& texManager) const;
   void processTextures(const aiMaterial* material, const aiTextureType texType, std::vector<MeshTexture>& textures, ResourceManager<Texture>& texManager) const;
};

#endif
