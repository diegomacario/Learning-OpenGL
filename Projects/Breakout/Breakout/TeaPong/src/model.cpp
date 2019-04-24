/*
#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

unsigned int TextureFromFile(const char *filename, const string &directory, bool gamma = false);

class Model
{
public:
   // Model Data
   vector<Texture> textures_loaded; // Stores all the textures loaded so far, so that each texture is not loaded more than once
   vector<Mesh>    meshes;
   string          directory;
   bool            gammaCorrection;

   // Constructor (expects a filepath to a 3D model)
   Model(string const &path, bool gamma = false)
      : gammaCorrection(gamma)
   {
      loadModel(path);
   }

   // Render the model
   void Draw(Shader shader)
   {
      for (unsigned int i = 0; i < meshes.size(); i++)
      {
         meshes[i].Draw(shader);
      }
   }

private:
   // Loads a model with supported ASSIMP extensions and stores the resulting meshes in the meshes vector
   void loadModel(string const &filepath)
   {
      // Read file via ASSIMP
      Assimp::Importer importer;
      const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

      // Check for errors
      if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
      {
         cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
         return;
      }

      // Retrieve the directory part of the filepath
      directory = filepath.substr(0, filepath.find_last_of('/'));

      // Process ASSIMP's root node recursively
      processNode(scene->mRootNode, scene);
   }

   // Processes the meshes stored in a node, and then repeats this process on the node's children
   void processNode(aiNode *node, const aiScene *scene)
   {
      // Process each mesh stored in the current node
      for (unsigned int i = 0; i < node->mNumMeshes; i++)
      {
         // A node only contains indices that can be used to access objects stored in the scene
         // Nodes are used to keep things organized by establishing parent-child relationships
         aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
         meshes.push_back(processMesh(mesh, scene));
      }

      // After we have processed all of the meshes stored in the current node, we recursively process its children
      for (unsigned int i = 0; i < node->mNumChildren; i++)
      {
         processNode(node->mChildren[i], scene);
      }
   }

   Mesh processMesh(aiMesh *mesh, const aiScene *scene)
   {
      // Data to fill
      vector<Vertex>       vertices;
      vector<unsigned int> indices;
      vector<Texture>      textures;

      // 1) Walk through each of the mesh's vertices
      for (unsigned int i = 0; i < mesh->mNumVertices; i++)
      {
         Vertex vertex;
         glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class, which doesn't convert directly to glm's vec3 class

         // Position
         vector.x = mesh->mVertices[i].x;
         vector.y = mesh->mVertices[i].y;
         vector.z = mesh->mVertices[i].z;
         vertex.Position = vector;

         // Normal
         vector.x = mesh->mNormals[i].x;
         vector.y = mesh->mNormals[i].y;
         vector.z = mesh->mNormals[i].z;
         vertex.Normal = vector;

         // Texture coordinates
         if (mesh->mTextureCoords[0]) // Check if the mesh contains texture coordinates
         {
            glm::vec2 vec;

            // A vertex can contain up to 8 different sets of texture coordinates
            // We make the assumption that we won't use models that have more than a single set of texture coordinates per vertex
            // For this reason, we always take the first set
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
         }
         else
         {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
         }

         // Tangent
         vector.x = mesh->mTangents[i].x;
         vector.y = mesh->mTangents[i].y;
         vector.z = mesh->mTangents[i].z;
         vertex.Tangent = vector;

         // Bitangent
         vector.x = mesh->mBitangents[i].x;
         vector.y = mesh->mBitangents[i].y;
         vector.z = mesh->mBitangents[i].z;
         vertex.Bitangent = vector;

         // Store the completed vertex
         vertices.push_back(vertex);
      }

      // 2) Walk through each of the mesh's faces and retrieve the corresponding vertex indices
      for (unsigned int i = 0; i < mesh->mNumFaces; i++)
      {
         aiFace face = mesh->mFaces[i];

         // Retrieve all the indices of the current face and store them in the indices vector
         for (unsigned int j = 0; j < face.mNumIndices; j++)
         {
            indices.push_back(face.mIndices[j]);
         }
      }

      // 3) Process the materials
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

      // We use a convention for sampler names in the shaders
      // Each diffuse texture should be named 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER
      // The same applies to other texture types, as the following list summarizes:
      // - Diffuse:  texture_diffuseN
      // - Specular: texture_specularN
      // - Normal:   texture_normalN

      // A. Diffuse maps
      vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

      // B. Specular maps
      vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

      // C. Normal maps
      std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
      textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

      // D. Height maps
      std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
      textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

      // Return a mesh object created from the extracted mesh data
      return Mesh(vertices, indices, textures);
   }

   // Walks through all the textures of a given type and loads the ones that have not been loaded
   vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
   {
      vector<Texture> textures;
      for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
      {
         aiString filename;
         mat->GetTexture(type, i, &filename);

         // Check if the current texture was loaded before, and if so, do not load it
         bool skip = false;
         for (unsigned int j = 0; j < textures_loaded.size(); j++)
         {
            if (std::strcmp(textures_loaded[j].filename.data(), filename.C_Str()) == 0)
            {
               textures.push_back(textures_loaded[j]);
               skip = true; // A texture with the same filepath has already been loaded, so we continue to next one
               break;
            }
         }

         if (!skip) // If the current texture hasn't been loaded already, load it
         {
            Texture texture;
            texture.id = TextureFromFile(filename.C_Str(), this->directory);
            texture.type = typeName;
            texture.filename = filename.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // Store the current texture in the model so that we can avoid loading it again
         }
      }

      return textures;
   }
};


unsigned int TextureFromFile(const char *filename, const string &directory, bool gamma)
{
   string filepath = string(filename);
   filepath = directory + '/' + filepath;

   unsigned int textureID;
   glGenTextures(1, &textureID);

   int width, height, nrComponents;
   unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
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
#endif
*/
