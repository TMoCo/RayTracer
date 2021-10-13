#ifndef SHADER_H
#define SHADER_H 1

#include <core/types.h>

#include <vector>

class Shader
{
private:
  UI32 id;

  bool valid;

  const char* getShaderCode(const char* path);

  UI32 checkShaderCompile(UI32 shader, const char* type);

public:
  void create(const char* vs_path, const char* fs_path); // basic create shader takes vs and fs arguments
  void use();

  // set uniforms ...
};

#endif // !SHADER_H
