#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:

   explicit Texture(GLuint texID);
   ~Texture();

   Texture(Texture&) = delete;
   Texture& operator=(Texture&) = delete;

   Texture(Texture&& rhs);
   Texture& operator=(Texture&& rhs);

   void Bind() const;

private:

   GLuint mTexID;
};

#endif
