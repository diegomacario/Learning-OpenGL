#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>

#include "model_loader.h"
#include "texture_loader.h"

Model ModelLoader::loadResource(const std::string& modelFilePath) const
{
   Assimp::Importer importer;
   const aiScene* scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs); // TODO: Allow the user to select which flags to use

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
   {
      std::cout << "Error - Model::loadModel - The error below occurred while importing this model: " << modelFilePath << "\n" << importer.GetErrorString() << "\n";
      return Model(std::vector<Mesh>());
   }

   mModelDir = modelFilePath.substr(0, modelFilePath.find_last_of('/'));

   std::vector<Mesh> meshes;
   ResourceManager<Texture> texManager;
   processNodeHierarchyRecursively(scene->mRootNode, scene, meshes, texManager);

   // TODO: Would it be possible to use move semantics to avoid copying the vector of meshes when creating the model? Or to optimize this in any other way?
   return Model(std::move(meshes));
}

void ModelLoader::processNodeHierarchyRecursively(const aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes, ResourceManager<Texture>& texManager) const
{
   // Create a Mesh object for each mesh referenced by the current node
   for (unsigned int i = 0; i < node->mNumMeshes; i++)
   {
      // Note that nodes do not store meshes directly
      // All the meshes are stored in the scene struct
      // Nodes only contain indices that can be used to access meshes from said struct
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene, texManager));
   }

   // After we have processed all the meshes referenced by the current node, we recursively process its children
   for (unsigned int i = 0; i < node->mNumChildren; i++)
   {
      processNodeHierarchyRecursively(node->mChildren[i], scene, meshes, texManager);
   }
}

Mesh ModelLoader::processMesh(const aiMesh* mesh, const aiScene* scene, ResourceManager<Texture>& texManager) const
{
   std::vector<Vertex>       vertices;
   std::vector<unsigned int> indices;
   std::vector<MeshTexture>  textures;

   processVertices(mesh, vertices);
   processIndices(mesh, indices);
   processMaterial(scene->mMaterials[mesh->mMaterialIndex], textures, texManager);

   // TODO: Could we take advantage of move semantics here?
   return Mesh(vertices, indices, std::move(textures));
}

void ModelLoader::processVertices(const aiMesh* mesh, std::vector<Vertex>& vertices) const
{
   vertices.reserve(mesh->mNumVertices);

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

      // TODO: Use emplace_back here?
      vertices.push_back(vertex);
   }
}

void ModelLoader::processIndices(const aiMesh* mesh, std::vector<unsigned int>& indices) const
{
   // We assume that mesh is made out of triangles
   // This will always be true if the aiProcess_Triangulate flag continues to be used when loading the model
   indices.reserve(mesh->mNumFaces * 3);

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

void ModelLoader::processMaterial(const aiMaterial* material, std::vector<MeshTexture>& textures, ResourceManager<Texture>& texManager) const
{
   // Process the material of the mesh
   // The material can consist of many textures of different types
   // We make the assumption that we will only use models that have ambient, emissive, diffuse and specular maps
   // TODO: Allow the user to select which texture types to process
   processTextures(material, aiTextureType_AMBIENT, textures, texManager);
   processTextures(material, aiTextureType_EMISSIVE, textures, texManager);
   processTextures(material, aiTextureType_DIFFUSE, textures, texManager);
   processTextures(material, aiTextureType_SPECULAR, textures, texManager);
}

void ModelLoader::processTextures(const aiMaterial* material, const aiTextureType texType, std::vector<MeshTexture>& textures, ResourceManager<Texture>& texManager) const
{
   std::string sampler2DUniformName;
   switch (texType)
   {
      // TODO: Would it be a good idea to somehow incorporate the filename of texture into our naming convention?
   case aiTextureType_AMBIENT:
      sampler2DUniformName = "ambientTex";
      break;
   case aiTextureType_EMISSIVE:
      sampler2DUniformName = "emissiveTex";
      break;
   case aiTextureType_DIFFUSE:
      sampler2DUniformName = "diffuseTex";
      break;
   case aiTextureType_SPECULAR:
      sampler2DUniformName = "specularTex";
      break;
   default:
      std::cout << "Error - ModelLoader::processTextures - Attempted to process textures of an invalid type: " << texType << "\n";
      return;
   }

   // Load all the textures of the specified type
   for (unsigned int i = 0; i < material->GetTextureCount(texType); i++)
   {
      aiString texFilename;
      material->GetTexture(texType, i, &texFilename);

      // Note that we assume that the textures are in the same directory as the model
      textures.emplace_back(texManager.loadResource<TextureLoader>(sampler2DUniformName + std::to_string(i), this->mModelDir + '/' + texFilename.C_Str()), texType, texFilename.C_Str());
   }
}
