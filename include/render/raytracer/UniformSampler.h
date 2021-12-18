/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Uniform sampling utility
//

#include <core/types.h>

class Vector3;
class Vector2;

class UniformSampler
{
public:
  static Vector2 disk(const Vector2& uv);

  static Vector3 hemisphere(const Vector2& uv);

  static Vector3 hemisphere(const Vector3& normal);

  static Vector3 hemisphere(const Vector2& uv, const Vector3& normal);

  static Vector3 unitSphere();

  static Vector3 unitSphere(const Vector2& uv);

  static Vector3 triangleBasuOwen(const F32& u); // slower but has better variance

  static Vector3 triangle(const Vector2& uv);

};
