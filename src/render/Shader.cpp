/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <render/Shader.h>

#include <resource/file.h>

#include <core/debug.h>

#include <glad/glad.h>

const std::vector<char> Shader::getShaderCode(const std::string& path)
{
  std::ifstream shaderStream(path, std::ios::ate);
  std::vector<char> data;

  if (!shaderStream.is_open())
  {
    DEBUG_PRINT("Could not open file stream\n");
  }

  UI32 len = static_cast<UI32>(shaderStream.tellg());
  data.resize(len);
  
  shaderStream.seekg(0);
  shaderStream.read(data.data(), len);
  
  shaderStream.close();

  return data;
}

UI32 Shader::checkShaderCompile(UI32 shader, const char* type)
{
  I32 success;
  char infoLog[file::MAX_LINE_SIZE];
  
  if(strcmp(type, "PROGRAM") != 0)
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, file::MAX_LINE_SIZE, NULL, infoLog);
      DEBUG_PRINT("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
      return 1;
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, file::MAX_LINE_SIZE, NULL, infoLog);
      DEBUG_PRINT("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
      return 1;
    }
  }
  return 0;
}

void Shader::create(const std::string& vs_path, const std::string& fs_path)
{
  const std::vector<char> vs_data = getShaderCode(vs_path);
  DEBUG_PRINT("%s\n", vs_data.data());

  const std::vector<char> fs_data = getShaderCode(fs_path);
  DEBUG_PRINT("%s\n", fs_data.data());
  
  UI32 vs = glCreateShader(GL_VERTEX_SHADER);
  const char* p_vs_data = vs_data.data();
  glShaderSource(vs, 1, &p_vs_data, NULL);
  glCompileShader(vs);
  checkShaderCompile(vs, "VERTEX");

  UI32 fs = glCreateShader(GL_FRAGMENT_SHADER);
  const char* p_fs_data = fs_data.data();
  glShaderSource(fs, 1, &p_fs_data, NULL);
  glCompileShader(fs);
  checkShaderCompile(fs, "FRAGMENT");

  id = glCreateProgram();
  glAttachShader(id, vs);
  glAttachShader(id, fs);
  glLinkProgram(id);

  if (checkShaderCompile(id, "PROGRAM") == 0)
  {
    valid = true;
  }

  glDeleteShader(vs);
  glDeleteShader(fs);
}

void Shader::use()
{
  if (valid)
  {
    glUseProgram(id);
  }
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

void Shader::setMatrix4(const char* name, const Matrix4& value) const
{
  glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, value[0]);
}
