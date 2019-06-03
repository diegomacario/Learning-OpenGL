#include <glad/glad.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shader_loader.h"

std::shared_ptr<Shader> ShaderLoader::loadResource(const std::string& vShaderFilePath, const std::string& fShaderFilePath) const
{
   GLuint vShaderID = createAndCompileShader(vShaderFilePath, GL_VERTEX_SHADER);
   GLuint fShaderID = createAndCompileShader(fShaderFilePath, GL_FRAGMENT_SHADER);

   GLuint shaderProgID = createAndLinkShaderProgram(vShaderID, fShaderID);

   glDeleteShader(vShaderID);
   glDeleteShader(fShaderID);

   return std::make_shared<Shader>(shaderProgID);
}

std::shared_ptr<Shader> ShaderLoader::loadResource(const std::string& vShaderFilePath, const std::string& fShaderFilePath, const std::string& gShaderFilePath) const
{
   GLuint vShaderID = createAndCompileShader(vShaderFilePath, GL_VERTEX_SHADER);
   GLuint fShaderID = createAndCompileShader(fShaderFilePath, GL_FRAGMENT_SHADER);
   GLuint gShaderID = createAndCompileShader(gShaderFilePath, GL_GEOMETRY_SHADER);

   GLuint shaderProgID = createAndLinkShaderProgram(vShaderID, fShaderID, gShaderID);

   glDeleteShader(vShaderID);
   glDeleteShader(fShaderID);
   glDeleteShader(gShaderID);

   return std::make_shared<Shader>(shaderProgID);
}

GLuint ShaderLoader::createAndCompileShader(const std::string& shaderFilePath, GLenum shaderType) const
{
   std::ifstream shaderFile(shaderFilePath);

   if (shaderFile)
   {
      // Read the entire file into a string
      std::stringstream shaderStream;
      shaderStream << shaderFile.rdbuf();
      std::string shaderCodeAsStr = shaderStream.str();

      shaderFile.close();

      const char* shaderCodeAsCStr  = shaderCodeAsStr.c_str();

      // Create and compile the shader
      GLuint shaderID = glCreateShader(shaderType);
      glShaderSource(shaderID, 1, &shaderCodeAsCStr, NULL);
      glCompileShader(shaderID);
      checkForCompilationErrors(shaderID, shaderType, shaderFilePath);
      return shaderID;
   }
   else
   {
      std::cout << "Error - The following shader file could not be opened: " << shaderFilePath << "\n";
      return 0;
   }
}

GLuint ShaderLoader::createAndLinkShaderProgram(GLuint vShaderID, GLuint fShaderID) const
{
   GLuint shaderProgID = glCreateProgram();

   glAttachShader(shaderProgID, vShaderID);
   glAttachShader(shaderProgID, fShaderID);

   glLinkProgram(shaderProgID);
   checkForLinkingErrors(shaderProgID);

   return shaderProgID;
}

GLuint ShaderLoader::createAndLinkShaderProgram(GLuint vShaderID, GLuint fShaderID, GLuint gShaderID) const
{
   GLuint shaderProgID = glCreateProgram();

   glAttachShader(shaderProgID, vShaderID);
   glAttachShader(shaderProgID, fShaderID);
   glAttachShader(shaderProgID, gShaderID);

   glLinkProgram(shaderProgID);
   checkForLinkingErrors(shaderProgID);

   return shaderProgID;
}

void ShaderLoader::checkForCompilationErrors(GLuint shaderID, GLenum shaderType, const std::string& shaderFilePath) const
{
   GLint success;
   glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

   if (!success)
   {
      GLint infoLogLength = 0;
      glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

      std::vector<GLchar> infoLog(infoLogLength);
      glGetShaderInfoLog(shaderID, infoLogLength, NULL, &infoLog[0]);

      std::cout << "Error - The error below occurred while compiling this shader: " << shaderFilePath << "\n" << infoLog.data() << "\n";
   }
}

void ShaderLoader::checkForLinkingErrors(GLuint shaderProgID) const
{
   GLint success;
   glGetProgramiv(shaderProgID, GL_LINK_STATUS, &success);

   if (!success)
   {
      GLint infoLogLength = 0;
      glGetProgramiv(shaderProgID, GL_INFO_LOG_LENGTH, &infoLogLength);

      std::vector<GLchar> infoLog(infoLogLength);
      glGetProgramInfoLog(shaderProgID, infoLogLength, NULL, &infoLog[0]);

      std::cout << "Error - The following error occurred while linking a shader program:\n" << infoLog.data() << "\n";
   }
}
