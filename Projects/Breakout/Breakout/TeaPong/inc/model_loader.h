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

   void processNodeHierarchyRecursively(const aiNode*             node,
                                        const aiScene*            scene,
                                        const std::string&        modelDir,
                                        ResourceManager<Texture>& texManager,
                                        std::vector<Mesh>&        meshes) const;

   // These methods translate Assimp's data into the Mesh objects that form the model
   std::vector<Vertex>       processVertices(const aiMesh* mesh) const;

   std::vector<unsigned int> processIndices(const aiMesh* mesh) const;

   std::vector<MeshTexture>  processMaterial(const aiMaterial*         material,
                                             const std::string&        modelDir,
                                             ResourceManager<Texture>& texManager) const;
};

#endif
