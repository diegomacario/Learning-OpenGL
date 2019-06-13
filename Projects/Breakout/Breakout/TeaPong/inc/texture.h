#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:

   Texture();

   void Bind() const;

private:

   GLuint mTexID;

   void Generate(GLuint width, GLuint height, unsigned char* data);
};

#endif
