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


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
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
    if (strcmp(token, "type") == 0)
    {
      token = strtok_s(remainding, " ", &remainding);
      // create material of desired type
      if (strcmp(token, "diffuse") == 0)
      {
        // extract diffuse colour
        token = strtok_s(remainding, " ", &remainding);
        if (!token)
        {
          material = ResourceManager::get().addMaterial(materialName, new Diffuse{ colour::Red });
        }
        else
        {
          float r = strtof(token, NULL), g = strtof(remainding, &token), b = strtof(token, NULL);
          material = ResourceManager::get().addMaterial(materialName, new Diffuse{ { r, g, b } });
        }
      }
      else if (strcmp(token, "dielectric") == 0)
      {
        token = strtok_s(remainding, " ", &remainding);
        if (!token)
        {
          material = ResourceManager::get().addMaterial(materialName, new Dielectric{ 1.5f });
        }
        else
        {
          float ior = strtof(token, NULL);
          material = ResourceManager::get().addMaterial(materialName, new Dielectric{ ior });
        }
      }
      else if (strcmp(token, "metal") == 0)
      {
        token = strtok_s(remainding, " ", &remainding);
        if (!token)
        {
          material = ResourceManager::get().addMaterial(materialName, new Metal{ 0.0f, { 0.8f, 0.8f, 0.8f } });
        }
        else
        {
          float fuzz = strtof(token, NULL);
          float r = strtof(remainding, &token), g = strtof(token, &token), b = strtof(token, NULL);
          material = ResourceManager::get().addMaterial(materialName, new Metal{ fuzz, { r, g, b } });
        }
      }
      else if (strcmp(token, "light") == 0)
      {
        token = strtok_s(remainding, " ", &remainding);
        if (!token)
        {
          material = ResourceManager::get().addMaterial(materialName, new DiffuseLight{ colour::White });
        }
        else
        {
          float r = strtof(token, NULL), g = strtof(remainding, &token), b = strtof(token, NULL);
          material = ResourceManager::get().addMaterial(materialName, new DiffuseLight{ { r, g, b } });
        }
      }
      else if (strcmp(token, "mixture") == 0)
      {
        material = ResourceManager::get().addMaterial(materialName, new Mixture);
      }
      continue;
    }

    if (strcmp(token, "albedoMap") == 0)
    {
      if (!material)
      {
        ERROR_MSG("Can't change material map properties of unspecified material!\nMake sure type is declared first.");
        return false;
      }

      // texture name
      token = strtok_s(remainding, " ", &remainding);

      // load texture
      material->setMap(ResourceManager::get().addTextureFromFile(token, token), MAT_MAPS::ALBEDO);
      continue;
    }

    if (strcmp(token, "metallicMap") == 0)
    {
      if (!material)
      {
        ERROR_MSG("Can't change material map properties of unspecified material!\nMake sure type is declared first.");
        return false;
      }

      // texture name
      token = strtok_s(remainding, " ", &remainding);

      // load texture
      material->setMap(ResourceManager::get().addTextureFromFile(token, token), MAT_MAPS::METALLIC);
      continue;
    }

    if (strcmp(token, "roughnessMap") == 0)
    {
      if (!material)
      {
        ERROR_MSG("Can't change material map properties of unspecified material!\nMake sure type is declared first.");
        return false;
      }

      // texture name
      token = strtok_s(remainding, " ", &remainding);

      // load texture
      material->setMap(ResourceManager::get().addTextureFromFile(token, token), MAT_MAPS::ROUGHNESS);
      continue;
    }

    if (strcmp(token, "normalMap") == 0)
    {
      if (!material)
      {
        ERROR_MSG("Can't change material map properties of unspecified material!\nMake sure type is declared first.");
        return false;
      }

      // texture name
      token = strtok_s(remainding, " ", &remainding);

      // load texture
      material->setMap(ResourceManager::get().addTextureFromFile(token, token), MAT_MAPS::NORMAL);
      continue;
    }

    if (strcmp(token, "emissiveMap") == 0)
    {
      if (!material)
      {
        ERROR_MSG("Can't change material map properties of unspecified material!\nMake sure type is declared first.");
        return false;
      }

      // texture name
      token = strtok_s(remainding, " ", &remainding);

      // load texture
      material->setMap(ResourceManager::get().addTextureFromFile(token, token), MAT_MAPS::EMISSIVE);
      continue;
    }

  }

  mtlStream.close();
  return 0;
}

bool MaterialLoader::loadTextureFromImageFile(const std::string& path, const std::string& textureName)
{
  int32_t width, height, channels;
  byte_t* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

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
      .addTexture(textureName, new Texture{ new Image(width, height, channels, data), format });
  }
  else
  {
    return false;
  }

  stbi_image_free(data);
  return true;
}
