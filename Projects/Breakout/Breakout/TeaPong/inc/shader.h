#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

#include <string>

class Shader
{
public:

   Shader(const std::string& vShaderFilePath, const std::string& fShaderFilePath);
   Shader(const std::string& vShaderFilePath, const std::string& fShaderFilePath, const std::string& gShaderFilePath);
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
   void   setVec4(const std::string& name, float x, float y, float z, float w) const;
          
   void   setMat2(const std::string& name, const glm::mat2& value) const;
   void   setMat3(const std::string& name, const glm::mat3& value) const;
   void   setMat4(const std::string& name, const glm::mat4& value) const;

private:

   GLuint mShaderProgID;

   GLuint createAndCompileShader(const std::string& shaderFilePath, GLenum shaderType) const;
   GLuint createAndLinkShaderProgram(GLuint vShaderID, GLuint fShaderID) const;
   GLuint createAndLinkShaderProgram(GLuint vShaderID, GLuint fShaderID, GLuint gShaderID) const;
   void   checkForCompilationErrors(GLuint shaderID, GLenum shaderType, const std::string& shaderFilePath) const;
   void   checkForLinkingErrors(GLuint shaderProgID) const;

   GLint  getUniformLocation(const std::string& name);
};

#endif
