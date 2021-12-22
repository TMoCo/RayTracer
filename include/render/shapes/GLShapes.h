/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Shapes data to be rendered in OpenGL
//

#ifndef GL_SHAPES_H
#define GL_SHAPES_H 1

#include <core/types.h>
#include <math/thomath.h>

class GLCube
{
  static GLCube* cube;
  
  UI32 VAO, VBO;

  GLCube()
  {
    // generate cube data
  }

public: 
  static GLCube* get()
  {
    if (!cube)
    {
      cube = new GLCube;
    }
    return cube;
  }

  static constexpr const UI32 indices[36] =
  {
    0, 2, 3,
    1, 2, 0,
    1, 2, 6,
    6, 5, 1,
    2, 3, 6,
    7, 6, 3,
    3, 0, 7,
    7, 0, 4,
    6, 7, 5,
    7, 4, 5,
    0, 1, 4,
    4, 1, 5
  };

  static constexpr const Vector3 unitCube[8] =
  {
    { -1.0f, 1.0f, 1.0f },
    { -1.0f, 1.0f, -1.0f },
    { 1.0f, 1.0f, -1.0f },
    { 1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f, 1.0f },
    { -1.0f, -1.0f, -1.0f },
    { 1.0f, -1.0f, -1.0f },
    { 1.0f, -1.0f, 1.0f },
  };


};

#endif // !GL_SHAPES_H

