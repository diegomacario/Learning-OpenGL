#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <array>
#include <iostream>

#include "model_loader.h"
#include "texture_loader.h"

std::shared_ptr<Model> ModelLoader::loadResource(const std::string& modelFilePath) const
{
   Assimp::Importer importer;
   const aiScene* scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs); // TODO: Allow the user to select which flags to use

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
   {
      std::cout << "Error - Model::loadModel - The error below occurred while importing this model: " << modelFilePath << "\n" << importer.GetErrorString() << "\n";
      return nullptr;
   }

   ResourceManager<Texture> texManager;
   std::vector<Mesh>        meshes;
   processNodeHierarchyRecursively(scene->mRootNode,
                                   scene,
                                   modelFilePath.substr(0, modelFilePath.find_last_of('/')),
                                   texManager,
                                   meshes);

   // TODO: Would it be possible to use move semantics to avoid copying the vector of meshes when creating the model? Or to optimize this in any other way?
   return std::make_shared<Model>(std::move(meshes), std::move(texManager));
}

void ModelLoader::processNodeHierarchyRecursively(const aiNode*             node,
                                                  const aiScene*            scene,
                                                  const std::string&        modelDir,
                                                  ResourceManager<Texture>& texManager,
                                                  std::vector<Mesh>&        meshes) const
{
   // Create a Mesh object for each mesh referenced by the current node
   for (unsigned int i = 0; i < node->mNumMeshes; i++)
   {
      // Note that nodes do not store meshes directly
      // All the meshes are stored in the scene struct
      // Nodes only contain indices that can be used to access meshes from said struct
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.emplace_back(processVertices(mesh),                                                           // Vertices
                          processIndices(mesh),                                                            // Indices
                          processMaterial(scene->mMaterials[mesh->mMaterialIndex], modelDir, texManager)); // Textures
   }

   // After we have processed all the meshes referenced by the current node, we recursively process its children
   for (unsigned int i = 0; i < node->mNumChildren; i++)
   {
      processNodeHierarchyRecursively(node->mChildren[i],
                                      scene,
                                      modelDir,
                                      texManager,
                                      meshes);
   }
}

std::vector<Vertex> ModelLoader::processVertices(const aiMesh* mesh) const
{
   std::vector<Vertex> vertices;
   vertices.reserve(mesh->mNumVertices);

   // Loop over the vertices of the mesh
   for (unsigned int i = 0; i < mesh->mNumVertices; i++)
   {
      // Store the position, the normal and the texture coordinates of the current vertex
      // Note that a vertex can contain up to 8 different sets of texture coordinates
      // We make the assumption that we will only use models that have a single set of texture coordinates per vertex
      // For this reason, we only check for the existence of the first set
      vertices.emplace_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),                                                // Position
                            glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),                                                   // Normal
                            mesh->HasTextureCoords(0) ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f, 0.0f)); // Texture coordinates
   }

   return vertices;
}

std::vector<unsigned int> ModelLoader::processIndices(const aiMesh* mesh) const
{
   // We assume that the mesh is made out of triangles, which is why we multiply the number of faces by 3 when reserving space in the indices vector
   // This will always be true if the aiProcess_Triangulate flag continues to be used when loading the model
   // TODO: Multiply by a different factor if we begin supporting other flags besides aiProcess_Triangulate
   std::vector<unsigned int> indices;
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

   return indices;
}

std::vector<MeshTexture> ModelLoader::processMaterial(const aiMaterial*         material,
                                                      const std::string&        modelDir,
                                                      ResourceManager<Texture>& texManager) const
{
   std::vector<MeshTexture> textures;

   // The material can consist of many textures of different types
   // We make the assumption that we will only use models that have ambient, emissive, diffuse and specular maps
   // TODO: Allow the user to select which texture types to process
   std::array<aiTextureType, 4> texTypes = {aiTextureType_AMBIENT,
                                            aiTextureType_EMISSIVE,
                                            aiTextureType_DIFFUSE,
                                            aiTextureType_SPECULAR};

   std::string uniformName;
   for (aiTextureType texType : texTypes)
   {
       // Compose the name of the sampler2D uniform that should exist in the shader
       // The numbering of the names starts at 0, so if we are processing 3 ambient textures, for example, the names of their corresponding sampler2D uniforms should be: ambientTex0, ambientTex1 and ambientTex2
       switch (texType)
       {
           // TODO: Would it be a good idea to somehow incorporate the filename of the texture into our naming convention?
       case aiTextureType_AMBIENT:
           uniformName = "ambientTex";
           break;
       case aiTextureType_EMISSIVE:
           uniformName = "emissiveTex";
           break;
       case aiTextureType_DIFFUSE:
           uniformName = "diffuseTex";
           break;
       case aiTextureType_SPECULAR:
           uniformName = "specularTex";
           break;
       default:
           std::cout << "Error - ModelLoader::processTextures - Attempted to process textures of an invalid type: " << texType << "\n";
           goto skip;
       }

       // Load all the textures of the specified type
       for (unsigned int i = 0; i < material->GetTextureCount(texType); i++)
       {
           aiString texFilename;
           material->GetTexture(texType, i, &texFilename);

           // Note that we assume that the textures are in the same directory as the model
           textures.emplace_back(texManager.loadResource<TextureLoader>(texFilename.C_Str(), modelDir + '/' + texFilename.C_Str()),
               uniformName + std::to_string(i));
       }

       // We jump here if we are asked to process textures of an invalid type
       skip:;
   }

   return textures;
}
