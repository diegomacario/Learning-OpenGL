#include <stb_image.h>

#include <iostream>

#include "texture_loader.h"

Texture TextureLoader::loadResource(const std::string& texFilePath,
                                    GLuint             wrapS,
                                    GLuint             wrapT,
                                    GLuint             minFilter,
                                    GLuint             magFilter,
                                    bool               genMipmap) const
{
   int width, height, numComponents;
   std::unique_ptr<unsigned char, void(*)(void*)> texData(stbi_load(texFilePath.c_str(), &width, &height, &numComponents, 0), stbi_image_free);

   if (!texData)
   {
      std::cout << "Error - TextureLoader::loadResource - The following texture could not be loaded: " << texFilePath << "\n";
      return Texture(0);
   }

   GLuint texID = generateTexture(texData, width, height, numComponents, wrapS, wrapT, minFilter, magFilter, genMipmap);

   return Texture(texID);
}

GLuint TextureLoader::generateTexture(const std::unique_ptr<unsigned char, void(*)(void*)>& texData,
                                      int    width,
                                      int    height,
                                      int    numComponents,
                                      GLuint wrapS,
                                      GLuint wrapT,
                                      GLuint minFilter,
                                      GLuint magFilter,
                                      bool   genMipmap) const
{
   GLenum format;
   switch (numComponents)
   {
   case 3:
      format = GL_RGB;
      break;
   case 4:
      format = GL_RGBA;
      break;
   default:
      std::cout << "Error - TextureLoader::generateTexture - The texture has an invalid number of components: " << numComponents << "\n";
   }

   GLuint texID;
   glGenTextures(1, &texID);
   glBindTexture(GL_TEXTURE_2D, texID);
   glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texData.get());

   if (genMipmap)
   {
      glGenerateMipmap(GL_TEXTURE_2D);
   }

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

   glBindTexture(GL_TEXTURE_2D, 0);

   return texID;
}
