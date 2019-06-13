#include <stb_image.h>

#include <iostream>

#include "texture_loader.h"

std::shared_ptr<Texture> TextureLoader::loadResource(const std::string& texFilePath) const
{
   GLuint texID = loadTexture(texFilePath,
                              GL_REPEAT,
                              GL_REPEAT,
                              GL_LINEAR,
                              GL_LINEAR,
                              false);
}

std::shared_ptr<Texture> TextureLoader::loadResource(const std::string& texFilePath,
                                                     GLuint             wrapS,
                                                     GLuint             wrapT,
                                                     GLuint             minFilter,
                                                     GLuint             magFilter,
                                                     bool               genMipmap) const
{
   GLuint texID = loadTexture(texFilePath,
                              wrapS,
                              wrapT,
                              minFilter,
                              magFilter,
                              genMipmap);
}

GLuint TextureLoader::loadTexture(const std::string& texFilePath,
                                  GLuint             wrapS,
                                  GLuint             wrapT,
                                  GLuint             minFilter,
                                  GLuint             magFilter,
                                  bool               genMipmap) const
{
   int width, height, numComponents;
   unsigned char* data = stbi_load(texFilePath.c_str(), &width, &height, &numComponents, 0);

   if (data)
   {
      GLenum format;
      switch (numComponents)
      {
      case 1:
         format = GL_RED;
         break;
      case 3:
         format = GL_RGB;
         break;
      case 4:
         format = GL_RGBA;
         break;
      default:
         std::cout << "Error - loadTexture - The following texture has an invalid number of components (" << numComponents << "): " << texFilePath << "\n";
         return 0;
      }

      GLuint texID;
      glGenTextures(1, &texID);
      glBindTexture(GL_TEXTURE_2D, texID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

      if (genMipmap)
      {
         glGenerateMipmap(GL_TEXTURE_2D);
      }

      // TODO: Allow the user to select which wrapping and sampling options to use
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

      stbi_image_free(data);

      return texID;
   }
   else
   {
      std::cout << "Error - loadTexture - The following texture could not be loaded: " << texFilePath << "\n";
      return 0;
   }
}
