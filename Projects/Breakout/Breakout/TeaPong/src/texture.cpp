#include <utility>

#include "texture.h"

Texture::Texture(GLuint texID)
   : mTexID(texID)
{

}

Texture::~Texture()
{
   glDeleteTextures(1, &mTexID);
}

Texture::Texture(Texture&& rhs) noexcept
   : mTexID(std::exchange(rhs.mTexID, 0))
{

}

Texture& Texture::operator=(Texture&& rhs) noexcept
{
   mTexID = std::exchange(rhs.mTexID, 0);
   return *this;
}

void Texture::Bind() const
{
   glBindTexture(GL_TEXTURE_2D, mTexID);
}
