#include "texture.h"

Texture::Texture(GLuint texID)
   : mTexID(texID)
{

}

void Texture::Bind() const
{
   glBindTexture(GL_TEXTURE_2D, mTexID);
}
