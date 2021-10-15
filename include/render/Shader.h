#ifndef SHADER_H
#define SHADER_H 1

#include <core/types.h>

#include <math/thomath.h>

struct Shader
{
  UI32 id;

  bool valid;

  const char* getShaderCode(const char* path);

  UI32 checkShaderCompile(UI32 shader, const char* type);

  void create(const char* vs_path, const char* fs_path); // basic create shader takes vs and fs arguments

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

#endif // !SHADER_H
