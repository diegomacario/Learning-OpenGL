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

   mModelFileDir = modelFilePath.substr(0, modelFilePath.find_last_of('/'));

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

   // Process the material of the mesh
   // The material can consist of many textures of different types
   // We make the assumption that we will only use models that have ambient, emissive, diffuse and specular maps
   aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

   // Store the ambient, emissive, diffuse and specular maps
   loadMaterialTextures(material, aiTextureType_AMBIENT, textures);
   loadMaterialTextures(material, aiTextureType_EMISSIVE, textures);
   loadMaterialTextures(material, aiTextureType_DIFFUSE, textures);
   loadMaterialTextures(material, aiTextureType_SPECULAR, textures);

   return Mesh(vertices, indices, textures);
}

void Model::loadMaterialTextures(const aiMaterial* material, const aiTextureType texType, std::vector<Texture>& textures)
{
   // Loop over all the textures of the specified texture type
   for (unsigned int i = 0; i < material->GetTextureCount(texType); i++)
   {
      aiString texFilename;
      material->GetTexture(texType, i, &texFilename);

      // Check if the current texture was loaded before, and if so, do not load it
      bool skip = false;
      for (unsigned int j = 0; j < mLoadedTextures.size(); j++)
      {
         if (std::strcmp(mLoadedTextures[j].filename.data(), texFilename.C_Str()) == 0)
         {
            textures.push_back(mLoadedTextures[j]);
            skip = true; // A texture with the same filepath has already been loaded, so we continue to next one
            break;
         }
      }

      if (!skip) // If the current texture hasn't been loaded already, load it
      {
         Texture texture;
         texture.id = TextureFromFile(texFilename.C_Str(), this->mModelFileDir);
         texture.type = texType;
         texture.filename = texFilename.C_Str();
         textures.push_back(texture);
         mLoadedTextures.push_back(texture); // Store the current texture in the model so that we can avoid loading it again
      }
   }
}

unsigned int TextureFromFile(const char* filename, const std::string& directory)
{
   std::string filepath = std::string(filename);
   filepath = directory + '/' + filepath;

   unsigned int textureID;
   glGenTextures(1, &textureID);

   int width, height, nrComponents;
   unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
   if (data)
   {
      GLenum format;
      if (nrComponents == 1)
      {
         format = GL_RED;
      }
      else if (nrComponents == 3)
      {
         format = GL_RGB;
      }
      else if (nrComponents == 4)
      {
         format = GL_RGBA;
      }

      glBindTexture(GL_TEXTURE_2D, textureID);
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
      std::cout << "Texture failed to load: " << filepath << std::endl;
      stbi_image_free(data);
   }

   return textureID;
}
