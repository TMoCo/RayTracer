/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// OBJLoader class definition
//

#include <render/materials/Material.h>
#include <resource/file.h>
#include <resource/OBJLoader.h>

#include <fstream>
#include <regex>

bool OBJLoader::loadObj(const std::string& fileName, const std::string& objectName, bool singleMesh)
{
  // HARD CODE SINGLE MESH
  singleMesh = true;

  if (!file::isOfType(fileName, ".obj"))
  {
    DEBUG_PRINT("File provided is not .obj");
    return false;
  }

  std::ifstream objStream(fileName);

  if (!objStream.is_open())
  {
    DEBUG_PRINT("Could not open file stream for file %s", fileName.c_str());
    return false;
  }

  std::string directory = file::getPath(fileName);

  // input processing
  std::string line;
  std::string::iterator lineIter;
  std::regex whitespace{ R"([\s]+)" };
  std::regex backslash { R"([/]+)" };

  // mesh data
  MeshBuilder meshBuilder;
  UI32 meshNum = 0;

  std::string meshName = objectName;
  if (meshName.empty())
  {
    meshName = file::getFileName(fileName);
  }

  // create new mesh and associate 
  if (singleMesh)
  {
    meshBuilder.reset(ResourceManager::get().addMesh(meshName, new Mesh));
  }

  // file data
  std::vector<F32> fPositions;
  std::vector<F32> fNormals;
  std::vector<Vector2> fUvs;

  // while there are lines available
  while (!objStream.eof())
  {
    std::getline(objStream, line);

    if (line.empty()) continue;

    // evaluate first character
    lineIter = line.begin();
    switch (*lineIter++)
    {
    case 'v':
    {
      // process a vertex
      char* next = nullptr;
      switch (*lineIter++)
      {
      case ' ':
        // vertex position
        fPositions.push_back(strtof(line.c_str() + 1, &next));
        fPositions.push_back(strtof(next, &next));
        fPositions.push_back(strtof(next, NULL));
        break;
      case 'n':
        // vertex normal
        fNormals.push_back(strtof(line.c_str() + 2, &next));
        fNormals.push_back(strtof(next, &next));
        fNormals.push_back(strtof(next, NULL));
        break;
      case 't':
        // vertex uv
        fUvs.push_back({ strtof(line.c_str() + 2, &next), strtof(next, NULL) });
        break;
      default:
        break;
      }
      break;
    }
    case 'f':
    {
      // tokenise face data by whitespace
      std::vector<std::string> faceData{
      std::sregex_token_iterator{ line.begin() + 2, line.end(), whitespace, -1 }, {} };
      DEBUG_ASSERT(faceData.size() < 5, "faces with more than 4 vertices are not currently supported");
            
      // face is be tokenised into an intermediary set of strings containing vertex data v/t/n 
      for (auto& vertex : faceData)
      {
        // if vertex does not exit already parse string
        if (!meshBuilder.uniqueIndices.count(vertex))
        {
          meshBuilder.uniqueIndices[vertex] = (UI32)(meshBuilder.uniqueIndices.size());
          
          std::vector<std::string> vertexData{ 
            std::sregex_token_iterator{ vertex.begin(), vertex.end(), backslash, -1 }, {} };

          size_t numVertexAttributes = vertexData.size();

          if (numVertexAttributes > 0) // position
          {
            size_t posIndex = stoull(vertexData[0]);
            meshBuilder.mesh->positions.push_back({ &fPositions[posIndex == 0 ? 0 : (posIndex - 1) * 3] });
          }
          if (numVertexAttributes > 1) // texture coordinate
          {
            size_t texIndex = stoull(vertexData[1]);
            meshBuilder.mesh->textureCoords.push_back(fUvs[texIndex == 0 ? 0 : texIndex - 1]);
          }
          if (numVertexAttributes > 2) // normal
          {
            size_t norIndex = stoull(vertexData[2]);
            meshBuilder.mesh->normals.push_back({ &fNormals[norIndex == 0 ? 0 : (norIndex - 1) * 3] });
          }
        }
        meshBuilder.mesh->indices.push_back(meshBuilder.uniqueIndices.at(vertex)); // get vertex index
      }
      if (faceData.size() == 4)
      {
        // insert two more vertices to complete quad
        meshBuilder.mesh->indices.push_back(meshBuilder.uniqueIndices.at(faceData[0]));
        meshBuilder.mesh->indices.push_back(meshBuilder.uniqueIndices.at(faceData[2]));
      }
    }
    break;
    case 'o':
      if (!singleMesh)
      {
        std::string subObjName = line.substr(0, 2); // eg: o teapot
        meshBuilder.reset(ResourceManager::get().
          addMesh(subObjName.empty() ? meshName + std::to_string(++meshNum) : meshName + subObjName, new Mesh));
      }
      break;
    case '#':
      // skip comment
      break;
    case '\n':
      // empty line
      break;
    default:
      // TODO: add material loading from file
      if (line.find("usemtl") != std::string::npos) break;
      if (line.find("mtllib") != std::string::npos) break;
      break;
    }
  }

  objStream.close();
  return true;
}  

bool OBJLoader::loadMtl(const std::string& path) 
{
  if (!file::isOfType(path, ".mtl")) 
  {
    DEBUG_PRINT("file provided is not .mtl");
    return false;
  }

  return true;
}
    /*
    FILE* pFile = std::fopen(path, "r");
    if (pFile == NULL)
        std::perror("Error opening file");

    // input
    char Image[MAX_LINE];
    char* token;

    Material* material = nullptr;

    while (std::fgets(Image, MAX_LINE, pFile) != NULL) {
        switch (Image[0]) {
        case 'N':
            switch (Image[1]) {
            case 's':
                token = std::strtok(&Image[3], " \n");
                material->specularExp = std::strtof(token, NULL);
                break;
            case 'i':
                token = std::strtok(&Image[3], " \n");
                material->ior = std::strtof(token, NULL);
                break;
            }
            break;
        case 'K':
            switch (Image[1]) {
            case 'a':
                material->ambient = Vector4{
                    std::strtof(&Image[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)};
                break;
            case 'd':
                material->diffuse = Vector4{
                    std::strtof(&Image[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)};
                break;
            case 's':
                material->specular = Vector4{
                    std::strtof(&Image[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)};
                break;
            case 'e':
                material->emissive = Vector4{
                    std::strtof(&Image[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)};
                break;
            }
        case 'd':
            material->dissolve = std::strtof(&Image[2], NULL);
            break;
        default:
            token = std::strtok(Image, " ");
            if (std::strcmp(token, "newmtl") == 0) {
                token = std::strtok(NULL, " \n");
                // insert a new material into the map
                material = &(model.materials[token]);
            }

            if (std::strcmp(token, "illum") == 0) {
                // illumination mode
                break;
            }
            break;
        }
    }

    std::fclose(pFile);
    */
