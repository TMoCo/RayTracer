/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Surfel declaration
//

#ifndef SURFEL_H
#define SURFEL_H

#include <math/thomath.h>

class Shape;

class Surfel 
{
public:
  Surfel() : shape(nullptr) { }

  Surfel(const Vector3& position, const Vector3& normal, const Vector3& outDirection, const Shape* shape)
    : position{ position }, normal{ normal }, outDirection{ outDirection }, shape{ shape }
  { }

  Vector3 position;
  
  Vector3 normal;

  Vector3 outDirection;

  const Shape* shape;
};

#endif // !SURFEL_H
