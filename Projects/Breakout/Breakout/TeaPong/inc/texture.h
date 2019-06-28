#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:

   explicit Texture(GLuint texID);
   ~Texture();

   Texture(const Texture&) = delete;
   Texture& operator=(const Texture&) = delete;

   Texture(Texture&& rhs) noexcept;
   Texture& operator=(Texture&& rhs) noexcept;

   void Bind() const;

private:

   GLuint mTexID;
};

#endif
