#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <memory>

#include "shader.h"

class ShaderLoader
{
public:

   ShaderLoader() = default;
   ~ShaderLoader() = default;

   ShaderLoader(const ShaderLoader&) = default;
   ShaderLoader& operator=(const ShaderLoader&) = default;

   ShaderLoader(ShaderLoader&&) = default;
   ShaderLoader& operator=(ShaderLoader&&) = default;

   std::shared_ptr<Shader> loadResource(const std::string& vShaderFilePath,
                                        const std::string& fShaderFilePath) const;

   std::shared_ptr<Shader> loadResource(const std::string& vShaderFilePath,
                                        const std::string& fShaderFilePath,
                                        const std::string& gShaderFilePath) const;

private:

   GLuint createAndCompileShader(const std::string& shaderFilePath, GLenum shaderType) const;
   GLuint createAndLinkShaderProgram(GLuint vShaderID, GLuint fShaderID) const;
   GLuint createAndLinkShaderProgram(GLuint vShaderID, GLuint fShaderID, GLuint gShaderID) const;
   void   checkForCompilationErrors(GLuint shaderID, GLenum shaderType, const std::string& shaderFilePath) const;
   void   checkForLinkingErrors(GLuint shaderProgID) const;
};

#endif
