#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:

   Texture(GLuint texID);

   void Bind() const;

private:

   GLuint mTexID;
};

#endif
