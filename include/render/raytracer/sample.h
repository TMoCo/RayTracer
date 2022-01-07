/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// sampling routines
//

#ifndef UNIFORM_SAMPLER_H
#define UNIFORM_SAMPLER_H 1

class Vector3;
class Vector2;

namespace sample
{
  // uniform 
  Vector2 uniform_disk(const Vector2& uv);

  Vector3 uniform_hemisphere(const Vector2& uv);

  Vector3 uniform_hemisphere(const Vector3& normal);

  Vector3 uniform_unitSphere();

  Vector3 uniform_unitSphere(const Vector2& uv);

  // uses the basu-owen method, slower but has better variance
  Vector3 uniform_triangle(const float& u);

  Vector3 uniform_triangle(const Vector2& uv);
}

#endif // !UNIFORM_SAMPLER_H