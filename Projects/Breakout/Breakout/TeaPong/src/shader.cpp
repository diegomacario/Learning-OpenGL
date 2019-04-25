#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "shader.h"

Shader::Shader(const std::string& vShaderFilePath, const std::string& fShaderFilePath)
{
   GLuint vShaderID = createAndCompileShader(vShaderFilePath, GL_VERTEX_SHADER);
   GLuint fShaderID = createAndCompileShader(fShaderFilePath, GL_FRAGMENT_SHADER);

   shaderProgID = createAndLinkShaderProgram(vShaderID, fShaderID);

   glDeleteShader(vShaderID);
   glDeleteShader(fShaderID);
}

Shader::Shader(const std::string& vShaderFilePath, const std::string& fShaderFilePath, const std::string& gShaderFilePath)
{
   GLuint vShaderID = createAndCompileShader(vShaderFilePath, GL_VERTEX_SHADER);
   GLuint fShaderID = createAndCompileShader(fShaderFilePath, GL_FRAGMENT_SHADER);
   GLuint gShaderID = createAndCompileShader(gShaderFilePath, GL_GEOMETRY_SHADER);

   shaderProgID = createAndLinkShaderProgram(vShaderID, fShaderID, gShaderID);

   glDeleteShader(vShaderID);
   glDeleteShader(fShaderID);
   glDeleteShader(gShaderID);
}

void Shader::use() const
{
   glUseProgram(shaderProgID);
}

GLuint Shader::getID() const
{
    return shaderProgID;
}

void Shader::setBool(const std::string& name, bool value) const
{
   glUniform1i(glGetUniformLocation(shaderProgID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
   glUniform1i(glGetUniformLocation(shaderProgID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
   glUniform1f(glGetUniformLocation(shaderProgID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2 &value) const
{
   glUniform2fv(glGetUniformLocation(shaderProgID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
   glUniform2f(glGetUniformLocation(shaderProgID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3 &value) const
{
   glUniform3fv(glGetUniformLocation(shaderProgID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
   glUniform3f(glGetUniformLocation(shaderProgID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4 &value) const
{
   glUniform4fv(glGetUniformLocation(shaderProgID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
   glUniform4f(glGetUniformLocation(shaderProgID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& value) const
{
   glUniformMatrix2fv(glGetUniformLocation(shaderProgID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) const
{
   glUniformMatrix3fv(glGetUniformLocation(shaderProgID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
   glUniformMatrix4fv(glGetUniformLocation(shaderProgID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

GLuint Shader::createAndCompileShader(const std::string& shaderFilePath, GLenum shaderType) const
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

GLuint Shader::createAndLinkShaderProgram(GLuint vShaderID, GLuint fShaderID) const
{
   GLuint shaderProgramID = glCreateProgram();

   glAttachShader(shaderProgramID, vShaderID);
   glAttachShader(shaderProgramID, fShaderID);

   glLinkProgram(shaderProgramID);
   checkForLinkingErrors(shaderProgramID);

   return shaderProgramID;
}

GLuint Shader::createAndLinkShaderProgram(GLuint vShaderID, GLuint fShaderID, GLuint gShaderID) const
{
   GLuint shaderProgramID = glCreateProgram();

   glAttachShader(shaderProgramID, vShaderID);
   glAttachShader(shaderProgramID, fShaderID);
   glAttachShader(shaderProgramID, gShaderID);

   glLinkProgram(shaderProgramID);
   checkForLinkingErrors(shaderProgramID);

   return shaderProgramID;
}

void Shader::checkForCompilationErrors(GLuint shaderID, GLenum shaderType, const std::string& shaderFilePath) const
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

void Shader::checkForLinkingErrors(GLuint shaderProgID) const
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
