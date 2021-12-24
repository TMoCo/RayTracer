/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <core/core.h>
#include <render/materials/materials.h>
#include <resource/file.h>
#include <resource/MaterialLoader.h>
#include <resource/ResourceManager.h>

#include <stb_image.h>
#include <stb_image_write.h>

bool MaterialLoader::loadMaterialFromFile(const std::string& path, const std::string& materialName)
{
  if (!file::isOfType(path, ".mtl"))
  {
    ERROR_MSG("FILE provided is not .mtl");
    return false;
  }

  std::ifstream mtlStream(path);

  if (!mtlStream.is_open())
  {
    ERROR_MSG("Could not open file stream for file %s", path);
    return false;
  }

  Material* material = nullptr;

  // input
  char buffer[file::MAX_LINE_SIZE];
  char *token, *remainding;
  while (!mtlStream.eof())
  {
    mtlStream.getline(buffer, file::MAX_LINE_SIZE);

    token = strtok_s(buffer, " ", &remainding);

    // analyse token
    if (strcmp(token, "type"))
    {
      token = strtok_s(buffer, " ", &remainding);
      // create material of desired type
      if (strcmp(token, "diffuse"))
      {
        // extract diffuse colour
        token = strtok_s(buffer, " ", &remainding);
        if (!token)
        {
          material = ResourceManager::get().addMaterial(materialName, new Diffuse{ colour::Red });
        }
        else
        {
          F32 r = strtof(token, &token), g = strtof(token, &token), b = strtof(token, NULL);
          material = ResourceManager::get().addMaterial(materialName, new Diffuse{ { r, g, b } });
        }

        continue;
      }
    }

    if (strcmp(token, "albedoMap") == 0)
    {
      if (!material)
      {
        ERROR_MSG("Can't change material properties to unspecified material!\nMake sure type is declared first.");
        return false;
      }

      // texture path
      token = strtok_s(buffer, " ", &remainding);

      // load texture
      material->setMap(ResourceManager::get().addTextureFromFile(token, token), MAT_MAPS::ALBEDO);
    }

  }

  mtlStream.close();
  return 0;
}

bool MaterialLoader::loadTextureFromImageFile(const std::string& path)
{
  I32 width, height, channels;
  byte* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

  if (data)
  {
    GLenum format = GL_NONE;
    switch (channels)
    {
    case 1:
      format = GL_RED;
      break;
    case 2:
      format = GL_RG;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      stbi_image_free(data);
      return false;
    }

    ResourceManager::get()
      .addTexture(path, new Texture{ new Image(width, height, channels, data), format });
  }
  else
  {
    return false;
  }

  stbi_image_free(data);
  return true;
}
