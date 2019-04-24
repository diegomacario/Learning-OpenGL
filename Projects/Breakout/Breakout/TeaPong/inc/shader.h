#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "glm/glm.hpp"

class Shader
{
public:

   Shader(const char* vShaderFilePath, const char* fShaderFilePath);
   Shader(const char* vShaderFilePath, const char* fShaderFilePath, const char* gShaderFilePath);
   // TODO: Add destructor that deletes the shader program?

   void   use() const;

   GLuint getID() const;

   void   setBool(const std::string& name, bool value) const;
   void   setInt(const std::string& name, int value) const;
   void   setFloat(const std::string& name, float value) const;

   void   setVec2(const std::string& name, const glm::vec2& value) const;
   void   setVec2(const std::string& name, float x, float y) const;
   void   setVec3(const std::string& name, const glm::vec3& value) const;
   void   setVec3(const std::string& name, float x, float y, float z) const;
   void   setVec4(const std::string& name, const glm::vec4& value) const;
   void   setVec4(const std::string& name, float x, float y, float z, float w);
          
   void   setMat2(const std::string& name, const glm::mat2& mat) const;
   void   setMat3(const std::string& name, const glm::mat3& mat) const;
   void   setMat4(const std::string& name, const glm::mat4& mat) const;

private:

   GLuint shaderProgID;

   GLuint createAndCompileShader(const char * shaderFilePath, GLenum shaderType) const;
   GLuint createAndLinkShaderProgram(GLuint vShaderID, GLuint fShaderID) const;
   GLuint createAndLinkShaderProgram(GLuint vShaderID, GLuint fShaderID, GLuint gShaderID) const;
   void   checkForCompilationErrors(GLuint shaderID, GLenum shaderType, const char* shaderFilePath) const;
   void   checkForLinkingErrors(GLuint shaderProgID) const;
};

#endif
