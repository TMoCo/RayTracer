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
class Material;

class Surfel 
{
public:
  Surfel() : shape{ nullptr }, material{ nullptr }, isFrontFace{ false } {}

  Surfel(const Vector3& position, const Vector3& normal, const Vector2& uv, const Vector3& outDirection, const Shape* shape)
    : position{ position }, normal{ normal }, uv{ uv }, outDirection{ outDirection }, shape{ shape }, material{ nullptr },
    isFrontFace{ normal.dot(-outDirection) < 0.0f }
  { 
    if (!isFrontFace)
    {
      this->normal *= -1.0f;
    }
    this->position += EPSILON * this->normal; // displace for shadow acne
  }


  Vector3 position;
  
  Vector3 normal;

  Vector3 outDirection;

  Vector2 uv;

  const Shape* shape;

  Material* material;

  bool isFrontFace;

};

#endif // !SURFEL_H
