//
// OBJLoader class definition
//

#include <render/Material.h>

#include <resource/file.h>
#include <resource/OBJLoader.h>

#include <fstream>
#include <regex>

bool OBJLoader::loadObj(const std::string& fileName, ResourceManager& resourceManager, bool singleMesh)
{
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

  // extract directory (if provided)
  std::string directory = file::getPath(fileName);

  // input processing
  std::string line;
  std::string::iterator lineIter;
  std::regex whitespace{ R"([\s]+)" };
  std::regex backslash { R"([/]+)" };

  // mesh data
  MeshBuilder meshBuilder;
  UI32 meshNum = 0;

  // create new mesh and associate 
  if (singleMesh)
  {
    resourceManager.meshes[fileName] = new Mesh; 
    meshBuilder.reset(resourceManager.meshes[fileName]);
  }

  // file data
  std::vector<F32> fPositions;
  std::vector<F32> fNormals;
  std::vector<Vector2> fUvs;

  // while there are lines available
  while (!objStream.eof())
  {
    std::getline(objStream, line);

    if (line.empty())
      continue;

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
        fPositions.push_back(std::strtof(line.c_str() + 1, &next));
        fPositions.push_back(std::strtof(next, &next));
        fPositions.push_back(std::strtof(next, NULL));
        break;
      case 'n':
        // vertex normal
        fNormals.push_back(std::strtof(line.c_str() + 2, &next));
        fNormals.push_back(std::strtof(next, &next));
        fNormals.push_back(std::strtof(next, NULL));
        break;
      case 't':
        // vertex uv
        fUvs.push_back({ std::strtof(line.c_str() + 2, &next), std::strtof(next, NULL) });
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
        // check if the vertex exists already
        if (!meshBuilder.uniqueIndices.count(vertex))
        {
          // if not, parse the string and create a new vertex in the mesh
          std::vector<std::string> vertexData{ 
            std::sregex_token_iterator{ vertex.begin(), vertex.end(), backslash, -1 }, {} };
          DEBUG_ASSERT(vertexData.size() == 3, "invalid face vertex format (expected v/t/n)");

          meshBuilder.uniqueIndices[vertex] = static_cast<UI32>(meshBuilder.uniqueIndices.size());

          I32 posIndex = std::stoi(vertexData[0]);
          I32 norIndex = std::stoi(vertexData[1]);
          I32 texIndex = std::stoi(vertexData[2]);

          // construct vector data types from floating point values
          meshBuilder.mesh->positions.push_back({ &fPositions[posIndex == 0 ? 0 : (posIndex - 1) * 3] });
          meshBuilder.mesh->normals.push_back({ &fNormals[texIndex == 0 ? 0 : (texIndex - 1) * 3] });
          // vector2 is 2 floats so just copy 
          meshBuilder.mesh->textureCoords.push_back(fUvs[norIndex == 0 ? 0 : norIndex - 1]);
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
      // create new mesh and associate it to the mesh builder
      if (!singleMesh)
      {
        std::string objectName = line.substr(0, 2);
        if (objectName.empty())
          objectName = meshNum++;
        // get object name and use with name for unique id
        std::string id = fileName + objectName;
        resourceManager.meshes[id] = new Mesh;
        meshBuilder.reset(resourceManager.meshes[id]);
      }
      break;
    case '#':
      // skip comment
      break;
    case '\n':
      // empty line
      break;
    default:
      // process a string 
      if (line.find("usemtl") != std::string::npos)
        break;
      if (line.find("mtllib") != std::string::npos)
        break;
      break;
    }
  }

  objStream.close();
  return true;
}  

bool OBJLoader::loadMtl(const std::string& path) {
  if (!file::isOfType(path, ".mtl")) {
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
    char buffer[MAX_LINE];
    char* token;

    Material* material = nullptr;

    while (std::fgets(buffer, MAX_LINE, pFile) != NULL) {
        switch (buffer[0]) {
        case 'N':
            switch (buffer[1]) {
            case 's':
                token = std::strtok(&buffer[3], " \n");
                material->specularExp = std::strtof(token, NULL);
                break;
            case 'i':
                token = std::strtok(&buffer[3], " \n");
                material->ior = std::strtof(token, NULL);
                break;
            }
            break;
        case 'K':
            switch (buffer[1]) {
            case 'a':
                material->ambient = Vector4{
                    std::strtof(&buffer[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)};
                break;
            case 'd':
                material->diffuse = Vector4{
                    std::strtof(&buffer[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)};
                break;
            case 's':
                material->specular = Vector4{
                    std::strtof(&buffer[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)};
                break;
            case 'e':
                material->emissive = Vector4{
                    std::strtof(&buffer[2], &token), 
                    std::strtof(token, &token), 
                    std::strtof(token, NULL)};
                break;
            }
        case 'd':
            material->dissolve = std::strtof(&buffer[2], NULL);
            break;
        default:
            token = std::strtok(buffer, " ");
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
