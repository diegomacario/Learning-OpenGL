#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
   unsigned int ID;

   Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
   {
      // 1) Retrieve the source code of the vertex and fragment shaders from their respective files
      // ****************************************************************************************************

      std::string vertexCode;
      std::string fragmentCode;
      std::string geometryCode;

      std::ifstream vShaderFile;
      std::ifstream fShaderFile;
      std::ifstream gShaderFile;

      // Ensure that the ifstream objects can throw exceptions:
      vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

      try
      {
         // Open the files
         vShaderFile.open(vertexPath);
         fShaderFile.open(fragmentPath);

         // Read the files' buffer contents into streams
         std::stringstream vShaderStream, fShaderStream;
         vShaderStream << vShaderFile.rdbuf();
         fShaderStream << fShaderFile.rdbuf();

         // Close the file handlers
         vShaderFile.close();
         fShaderFile.close();

         // Convert the streams into strings
         vertexCode   = vShaderStream.str();
         fragmentCode = fShaderStream.str();

         // If the path of the geometry shader is present, also load it
         if (geometryPath != nullptr)
         {
            // Open the file
            gShaderFile.open(geometryPath);

            // Read the file's buffer content into a stream
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();

            // Close the file handler
            gShaderFile.close();

            // Convert the stream into a string
            geometryCode = gShaderStream.str();
         }
      }
      catch (std::ifstream::failure e)
      {
         std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
      }

      // Convert the strings into C-strings
      const char* vShaderCode  = vertexCode.c_str();
      const char * fShaderCode = fragmentCode.c_str();

      // 2) Compile the shaders
      // ****************************************************************************************************

      unsigned int vertex, fragment;

      // Vertex shader
      vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vShaderCode, NULL);
      glCompileShader(vertex);
      checkCompileErrors(vertex, "VERTEX");

      // Fragment Shader
      fragment = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment, 1, &fShaderCode, NULL);
      glCompileShader(fragment);
      checkCompileErrors(fragment, "FRAGMENT");

      // If the path of the geometry shader is present, also compile it
      unsigned int geometry;
      if (geometryPath != nullptr)
      {
         const char * gShaderCode = geometryCode.c_str();
         geometry = glCreateShader(GL_GEOMETRY_SHADER);
         glShaderSource(geometry, 1, &gShaderCode, NULL);
         glCompileShader(geometry);
         checkCompileErrors(geometry, "GEOMETRY");
      }

      // 3) Create the shader program, attach the shaders to it, and link it
      // ****************************************************************************************************

      // Create the shader program
      ID = glCreateProgram();

      // Attach the shaders to the program
      glAttachShader(ID, vertex);
      glAttachShader(ID, fragment);
      if (geometryPath != nullptr)
      {
         glAttachShader(ID, geometry);
      }

      // Link the program
      glLinkProgram(ID);
      checkCompileErrors(ID, "PROGRAM");

      // Delete the shaders
      glDeleteShader(vertex);
      glDeleteShader(fragment);
      if (geometryPath != nullptr)
      {
         glDeleteShader(geometry);
      }
   }

   void use()
   {
      // Activate the shader
      glUseProgram(ID);
   }

   void setBool(const std::string &name, bool value) const
   {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
   }

   void setInt(const std::string &name, int value) const
   {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
   }

   void setFloat(const std::string &name, float value) const
   {
      glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
   }

   void setVec2(const std::string &name, const glm::vec2 &value) const
   {
      glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
   }

   void setVec2(const std::string &name, float x, float y) const
   {
      glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
   }

   void setVec3(const std::string &name, const glm::vec3 &value) const
   {
      glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
   }

   void setVec3(const std::string &name, float x, float y, float z) const
   {
      glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
   }

   void setVec4(const std::string &name, const glm::vec4 &value) const
   {
      glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
   }

   void setVec4(const std::string &name, float x, float y, float z, float w)
   {
      glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
   }

   void setMat2(const std::string &name, const glm::mat2 &mat) const
   {
      glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
   }

   void setMat3(const std::string &name, const glm::mat3 &mat) const
   {
      glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
   }

   void setMat4(const std::string &name, const glm::mat4 &mat) const
   {
      glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
   }

private:

   void checkCompileErrors(GLuint shader, std::string type)
   {
      GLint success;
      GLchar infoLog[1024];
      if (type != "PROGRAM")
      {
         glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
         if (!success)
         {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
         }
      }
      else
      {
         glGetProgramiv(shader, GL_LINK_STATUS, &success);
         if (!success)
         {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
         }
      }
   }
};

#endif