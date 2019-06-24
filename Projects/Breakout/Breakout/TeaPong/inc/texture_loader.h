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

   TextureLoader(const TextureLoader&) = delete;
   TextureLoader& operator=(const TextureLoader&) = delete;

   TextureLoader(TextureLoader&&) = delete;
   TextureLoader& operator=(TextureLoader&&) = delete;

   Texture loadResource(const std::string& texFilePath,
                        GLuint             wrapS     = GL_REPEAT,
                        GLuint             wrapT     = GL_REPEAT,
                        GLuint             minFilter = GL_LINEAR,
                        GLuint             magFilter = GL_LINEAR,
                        bool               genMipmap = false) const;

private:

   GLuint generateTexture(const std::unique_ptr<unsigned char, void(*)(void*)>& texData,
                          int    width,
                          int    height,
                          int    numComponents,
                          GLuint wrapS,
                          GLuint wrapT,
                          GLuint minFilter,
                          GLuint magFilter,
                          bool   genMipmap) const;
};

#endif
