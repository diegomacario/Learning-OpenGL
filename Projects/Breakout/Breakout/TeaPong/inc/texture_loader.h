#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>
#include <memory>

#include "texture.h"

class TextureLoader
{
public:

   TextureLoader() = default;
   ~TextureLoader() = default;

   TextureLoader(TextureLoader&) = delete;
   TextureLoader& operator=(TextureLoader&) = delete;

   TextureLoader(TextureLoader&&) = delete;
   TextureLoader& operator=(TextureLoader&&) = delete;

   std::shared_ptr<Texture> loadResource(const std::string& texFilePath) const;
   std::shared_ptr<Texture> loadResource(const std::string& texFilePath, GLuint wrapS, GLuint wrapT, GLuint minFilter, GLuint maxFilter) const;

private:

   GLuint loadTexture(const std::string& texFilePath);

};

#endif
