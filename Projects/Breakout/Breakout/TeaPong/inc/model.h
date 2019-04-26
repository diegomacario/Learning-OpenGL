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

   void draw(Shader shader);

private:

   std::string          modelFileDir;
   std::vector<Mesh>    meshes;
   std::vector<Texture> loadedTextures; // Stores all the textures loaded so far, so that each texture is not loaded more than once

   // Loads a model with supported ASSIMP extensions and stores the resulting meshes in the meshes vector
   void loadModel(const std::string& modelFilePath);

   // Processes the meshes stored in a node, and then repeats this process on the node's children
   void processNode(aiNode* node, const aiScene* scene);

   Mesh processMesh(aiMesh* mesh, const aiScene* scene);

   // Walks through all the textures of a given type and loads the ones that have not been loaded
   std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

unsigned int TextureFromFile(const char* filename, const std::string& directory);

#endif
