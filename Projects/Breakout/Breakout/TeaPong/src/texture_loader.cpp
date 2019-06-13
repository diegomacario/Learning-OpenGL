#include <stb_image.h>

#include <iostream>

#include "texture_loader.h"

std::shared_ptr<Texture> TextureLoader::loadResource(const std::string& texFilePath) const
{
   GLuint texID;
   glGenTextures(1, &texID);
}

std::shared_ptr<Texture> TextureLoader::loadResource(const std::string& texFilePath, GLuint wrapS, GLuint wrapT, GLuint minFilter, GLuint maxFilter) const
{

}

GLuint TextureLoader::loadTexture(const std::string& texFilePath)
{
   GLuint texID;
   glGenTextures(1, &texID);

   int width, height, numComponents;
   unsigned char* data = stbi_load(texFilePath.c_str(), &width, &height, &numComponents, 0);

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
      default: std::cout << "Error - loadTexture - The following texture has an invalid number of components (" << numComponents << "): " << texFilePath << "\n";
         break;
      }

      glBindTexture(GL_TEXTURE_2D, texID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      // TODO: Allow the user to select which wrapping and sampling options to use
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
   }
   else
   {
      std::cout << "Error - loadTexture - The following texture could not be loaded: " << texFilePath << "\n";
   }

   return texID;
}
