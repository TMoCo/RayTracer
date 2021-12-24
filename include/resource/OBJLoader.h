/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// OBJLoader class declaration
//

#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H 1

#include <string>
#include <unordered_map>

class Mesh;

class OBJLoader 
{
public:
  static bool loadOBJFromFile(const std::string& path, const std::string& objectName, bool singleMesh);

private:
  struct MeshBuilder
  {
    MeshBuilder() 
      : mesh{ nullptr } 
    { }

    inline void reset(Mesh* m = nullptr)
    {
      uniqueIndices.clear();
      mesh = m; // if provided
    }

    Mesh* mesh;

    std::unordered_map<std::string, UI32> uniqueIndices;

  };    

};


#endif // ! OBJ_LOADER_H
 