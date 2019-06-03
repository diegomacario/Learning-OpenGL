#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "shader.h"
#include "resource_loader.h"

class ShaderLoader
{
public:

   ShaderLoader() = default;
   ~ShaderLoader() = default;

   ShaderLoader(ShaderLoader&) = delete;
   ShaderLoader& operator=(ShaderLoader&) = delete;

   ShaderLoader(ShaderLoader&&) = delete;
   ShaderLoader& operator=(ShaderLoader&&) = delete;

   std::shared_ptr<Shader> loadResource(const std::string& vShaderFilePath, const std::string& fShaderFilePath) const;
   std::shared_ptr<Shader> loadResource(const std::string& vShaderFilePath, const std::string& fShaderFilePath, const std::string& gShaderFilePath) const;

private:

   GLuint createAndCompileShader(const std::string& shaderFilePath, GLenum shaderType) const;
   GLuint createAndLinkShaderProgram(GLuint vShaderID, GLuint fShaderID) const;
   GLuint createAndLinkShaderProgram(GLuint vShaderID, GLuint fShaderID, GLuint gShaderID) const;
   void   checkForCompilationErrors(GLuint shaderID, GLenum shaderType, const std::string& shaderFilePath) const;
   void   checkForLinkingErrors(GLuint shaderProgID) const;
};

#endif
