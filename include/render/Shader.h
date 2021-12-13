/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/#ifndef SHADER_H_
#define SHADER_H_ 1

#include <core/types.h>

#include <math/thomath.h>

#include <vector>

struct Shader
{
  UI32 id;

  bool valid;

  const std::vector<char> getShaderCode(const std::string& path);

  UI32 checkShaderCompile(UI32 shader, const char* type);

  void create(const std::string& vs_path, const std::string& fs_path); // basic create shader takes vs and fs arguments

  void use();

  // set uniforms ...
  void setBool(const char* name, bool value) const;

  void setInt(const char* name, I32 value) const; 

  void setFloat(const char* name, F32 value) const;

  void setVec2(const char* name, const Vector2& value) const;
  
  void setVec3(const char* name, const Vector3& value) const;

  void setVec4(const char* name, const Vector4& value) const;

  void setMatrix4(const char* name, const Matrix4& value) const;
};

#endif // !SHADER_H_
