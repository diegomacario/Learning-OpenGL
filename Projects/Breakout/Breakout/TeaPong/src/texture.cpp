#include "texture.h"

Texture::Texture(GLuint texID)
   : mTexID(texID)
{

}

Texture::~Texture()
{
   glDeleteTextures(1, &mTexID);
}

void Texture::Bind() const
{
   glBindTexture(GL_TEXTURE_2D, mTexID);
}
