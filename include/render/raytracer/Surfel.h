/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Surfel declaration
//

#ifndef SURFEL_H_
#define SURFEL_H_

#include <math/thomath.h>

#include <core/core.h>

#include <render/primitives/Mesh.h>

#include <image/Colour.h>

class Surfel {
public:
  Surfel() {}

  Surfel(const Vector3& position, const Vector2& UV, const Vector3& outDirection, 
    const Vector3& dPdU, const Vector3& dPdV, const Vector3& dNdU, const Vector3& dNdV, const Shape* shape)
    : position{ position }, normal{ dPdU.cross(dPdV).normalize() }, outDirection{ outDirection }, UV{ UV }, 
    dPdU{ dPdU }, dPdV{ dPdV }, dNdU{ dNdU }, dNdV{ dNdV }, shape{ shape }
  {
    // reverse normals? shading geometry?
  }

  Vector3 position;
  
  Vector3 normal;

  Vector3 outDirection;

  Vector2 UV;

  Vector3 dPdU, dPdV; // partial derivatives of point p with respect to u and v
  Vector3 dNdU, dNdV; // ditto for normal

  const Shape* shape = nullptr;
};

#endif // !SURFEL_H_
