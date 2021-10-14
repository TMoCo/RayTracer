#include <render/Shader.h>

#include <core/debug.h>
#include <core/core.h>

#include <fstream>

#include <glad/glad.h>

const char* Shader::getShaderCode(const char* path)
{
  std::ifstream shaderStream(path, std::ios::ate);

  if (!shaderStream.is_open())
  {
    DEBUG_PRINT("Could not open file stream\n");
    return NULL; // empty 
  }

  UI32 len = static_cast<UI32>(shaderStream.tellg()); // get size of file
  shaderStream.seekg(0); // reset to begining of stream

  char* shader = new char[len];
  memset(shader, '\0', len);

  shaderStream.read(shader, len);

  shaderStream.close();

  return shader;
}

UI32 Shader::checkShaderCompile(UI32 shader, const char* type)
{
  I32 success;
  char infoLog[MAX_LINE];
  if(strcmp(type, "PROGRAM") != 0)
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, MAX_LINE, NULL, infoLog);
      DEBUG_PRINT("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
      return 1;
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, MAX_LINE, NULL, infoLog);
      DEBUG_PRINT("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
      return 1;
    }
  }
  return 0;
}

void Shader::create(const char* vs_path, const char* fs_path)
{
  // get shader code
  const char* vs_data = getShaderCode(vs_path);
  DEBUG_PRINT("%s\n", vs_data);

  const char* fs_data = getShaderCode(fs_path);
  DEBUG_PRINT("%s\n", fs_data);
  
  // compile shaders
  UI32 vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vs_data, NULL);
  glCompileShader(vs);
  checkShaderCompile(vs, "VERTEX");
  delete[] vs_data;

  UI32 fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fs_data, NULL);
  glCompileShader(fs);
  checkShaderCompile(fs, "FRAGMENT");
  delete[] fs_data;

  // create program
  id = glCreateProgram();
  glAttachShader(id, vs);
  glAttachShader(id, fs);
  glLinkProgram(id);

  if (checkShaderCompile(id, "PROGRAM") == 0)
    valid = true;

  // cleanup
  glDeleteShader(vs);
  glDeleteShader(fs);
}

void Shader::use()
{
  if (valid)
    glUseProgram(id);
}

void Shader::setBool(const char* name, bool value) const
{
  glUniform1i(glGetUniformLocation(id, name), (I32)value);
}

void Shader::setInt(const char* name, I32 value) const
{
  glUniform1i(glGetUniformLocation(id, name), value);
}


void Shader::setFloat(const char* name, F32 value) const
{
  glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::setVec2(const char* name, const Vector2& value) const
{
  glUniform2fv(glGetUniformLocation(id, name), 1, value._v);
}

void Shader::setVec3(const char* name, const Vector3& value) const
{
  glUniform3fv(glGetUniformLocation(id, name), 1, value._v);
}

void Shader::setVec4(const char* name, const Vector4& value) const
{
  glUniform4fv(glGetUniformLocation(id, name), 1, value._v);
}

void Shader::setMat4(const char* name, const Matrix4& value) const
{
  glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, value[0]);
}
