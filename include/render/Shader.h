/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#ifndef SHADER_H
#define SHADER_H 1

#include <core/types.h>
#include <Vector3.h>
#include <Vector2.h>
#include <Matrix4.h>

#include <vector>

class Shader
{
public:
  Shader();

  Shader(const std::string& vs_path, const std::string& fs_path);

  ~Shader();

  void use();

  bool isValid() const;

  void setBool(const char* name, bool value) const;

  void setInt(const char* name, int value) const; 

  void setFloat(const char* name, float value) const;

  void setVec2(const char* name, const Vector2& value) const;
  
  void setVec3(const char* name, const Vector3& value) const;

  void setVec4(const char* name, const Vector4& value) const;

  void setMatrix4(const char* name, const Matrix4& value) const;

private:
  const std::vector<char> getShaderCode(const std::string& path);

  int checkShaderCompile(uint32_t shader, const char* type);

  uint32_t id;

  bool valid;
};

#endif // !SHADER_H
