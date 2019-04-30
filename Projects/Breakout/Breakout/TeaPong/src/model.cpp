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

void Model::draw(const Shader& shader) const
{
   for (auto &mesh : mMeshes)
   {
      mesh.draw(shader);
   }
}

void Model::loadModel(const std::string& modelFilePath)
{
   Assimp::Importer importer;

   const aiScene* scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs); // TODO: Allow user to select which flags to use

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
   {
      std::cout << "Error - The error below occurred while importing this model: " << modelFilePath << "\n" << importer.GetErrorString() << "\n";
      return;
   }

   mModelDir = modelFilePath.substr(0, modelFilePath.find_last_of('/'));

   processNodeHierarchyRecursively(scene->mRootNode, scene);
}

void Model::processNodeHierarchyRecursively(const aiNode* node, const aiScene* scene)
{
   // Create a Mesh object for each mesh referenced by the current node
   for (unsigned int i = 0; i < node->mNumMeshes; i++)
   {
      // Note that nodes do not store meshes directly
      // All the meshes are stored in the scene struct
      // Nodes only contain indices that can be used to access meshes from said struct
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      mMeshes.push_back(processMesh(mesh, scene));
   }

   // After we have processed all the meshes referenced by the current node, we recursively process its children
   for (unsigned int i = 0; i < node->mNumChildren; i++)
   {
      processNodeHierarchyRecursively(node->mChildren[i], scene);
   }
}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene)
{
   std::vector<Vertex>       vertices;
   std::vector<unsigned int> indices;
   std::vector<Texture>      textures;

   processVertices(mesh, vertices);
   processIndices(mesh, indices);
   processMaterial(scene->mMaterials[mesh->mMaterialIndex], textures);

   return Mesh(vertices, indices, textures);
}

void Model::processVertices(const aiMesh* mesh, std::vector<Vertex>& vertices)
{
   // Loop over the vertices of the mesh
   for (unsigned int i = 0; i < mesh->mNumVertices; i++)
   {
      Vertex vertex;

      // Store the position, the normal and the texture coordinates of the current vertex
      // Note that a vertex can contain up to 8 different sets of texture coordinates
      // We make the assumption that we will only use models that have a single set of texture coordinates per vertex
      // For this reason, we only check for the existence of the first set
      vertex.position  = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
      vertex.normal    = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
      vertex.texCoords = mesh->HasTextureCoords(0) ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f, 0.0f);

      vertices.push_back(vertex);
   }
}

void Model::processIndices(const aiMesh* mesh, std::vector<unsigned int>& indices)
{
   // Loop over the faces of the mesh
   for (unsigned int i = 0; i < mesh->mNumFaces; i++)
   {
      aiFace face = mesh->mFaces[i];

      // Store the indices of the current face
      for (unsigned int j = 0; j < face.mNumIndices; j++)
      {
         indices.push_back(face.mIndices[j]);
      }
   }
}

void Model::processMaterial(const aiMaterial* material, std::vector<Texture>& textures)
{
   // Process the material of the mesh
   // The material can consist of many textures of different types
   // We make the assumption that we will only use models that have ambient, emissive, diffuse and specular maps
   processTextures(material, aiTextureType_AMBIENT, textures);
   processTextures(material, aiTextureType_EMISSIVE, textures);
   processTextures(material, aiTextureType_DIFFUSE, textures);
   processTextures(material, aiTextureType_SPECULAR, textures);
}

void Model::processTextures(const aiMaterial* material, const aiTextureType texType, std::vector<Texture>& textures)
{
   // Loop over all the textures of the specified type
   for (unsigned int i = 0; i < material->GetTextureCount(texType); i++)
   {
      aiString texFilename;
      material->GetTexture(texType, i, &texFilename);

      // Check if a texture with the same name as the current one has been loaded before
      std::map<std::string, Texture>::iterator it = mLoadedTextures.find(texFilename.C_Str());
      if (it != mLoadedTextures.end())
      {
         // Since the texture has been loaded before, we simply take it from the mLoadedTextures map and add it to the mesh
         textures.push_back(it->second);
      }
      else
      {
         // Since the texture has not been loaded before, we load it and add it to both the mLoadedTextures map and to the mesh
         Texture texture;
         texture.id       = loadTexture(texFilename.C_Str(), this->mModelDir);
         texture.type     = texType;
         texture.filename = texFilename.C_Str();

         textures.push_back(texture);

         mLoadedTextures[texFilename.C_Str()] = texture;
      }
   }
}

unsigned int loadTexture(const std::string& texFilename, const std::string& modelDir)
{
   // We assume that textures are in the same directory as the model
   std::string filePath = modelDir + '/' + texFilename;

   unsigned int texID;
   glGenTextures(1, &texID);

   int width, height, numComponents;
   unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &numComponents, 0);

   if (data)
   {
      GLenum format;
      switch (numComponents)
      {
         case 1: format = GL_RED;
            break;
         case 3: format = GL_RGB;
            break;
         case 4: format = GL_RGBA;
            break;
      }

      glBindTexture(GL_TEXTURE_2D, texID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
   }
   else
   {
      std::cout << "Error - The following texture could not be loaded: " << filePath << "\n";
   }

   return texID;
}
