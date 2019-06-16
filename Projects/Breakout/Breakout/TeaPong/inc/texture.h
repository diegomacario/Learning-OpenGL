#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:

   explicit Texture(GLuint texID);
   // TODO: Add destructor that deletes the texture?

   void Bind() const;

private:

   GLuint mTexID;
};

#endif
