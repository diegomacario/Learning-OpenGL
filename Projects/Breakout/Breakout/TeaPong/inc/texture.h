#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:

   explicit Texture(GLuint texID);
   ~Texture();

   // TODO: Should it be possible to copy a texture? Would that result in two textures pointing to the same memory in the GPU? What happens if glDeleteTextures gets called twice?
   Texture(Texture&) = default;
   Texture& operator=(Texture&) = default;

   Texture(Texture&&) = default;
   Texture& operator=(Texture&&) = default;

   void Bind() const;

private:

   GLuint mTexID;
};

#endif
