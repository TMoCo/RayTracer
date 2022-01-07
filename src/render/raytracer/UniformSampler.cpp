/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <core/random.h>
#include <constants.h>
#include <render/raytracer/UniformSampler.h>

Vector2 UniformSampler::disk(const Vector2& uv)
{
  // https://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/2D_Sampling_with_Multidimensional_Transformations
  Vector2 p = { uv[0] * 2.0f - 1.0f, uv[1] * 2.0f - 1.0f }; // [0,1] to range [-1,1]
  if (p[0] + p[1] == 0)
  {
    return Vector2{}; // degenerate case
  }

  // generate a random point on a unit disk
  float r, theta;
  if (abs(p[0]) > abs(p[1]))
  {
    r = p[0];
    // distorts grid to avoid collecting at origin
    theta = PI_FOUR * (p[1] / p[0]);
  }
  else
  {
    r = p[1];
    theta = PI_TWO - PI_FOUR * (p[0] / p[1]);
  }

  return r * Vector2{ cosf(theta), sinf(theta) };
}

Vector3 UniformSampler::hemisphere(const Vector2& uv)
{
  // generate a random vector on the unit hemisphere
  float z = uv[0];
  float r = sqrtf(fmax(0.0f, 1.0f - z * z));
  float phi = TWO_PI * uv[1];
  return { r * cosf(phi), r * sinf(phi), z };
}

Vector3 UniformSampler::hemisphere(const Vector3& normal)
{
  Vector3 inUnitSphere = (normal + unitSphere(random::uniformUV())).normalize();
  return inUnitSphere.dot(normal) > 0.0f ? inUnitSphere : -inUnitSphere;
}

Vector3 UniformSampler::unitSphere()
{
  Vector3 vector;
  while (true)
  {
    vector = random::uniformVector3(-1.0f, 1.0f);
    if (vector.lengthSquared() >= 1.0f)
    {
      continue;
    }
    return vector;
  }
}

Vector3 UniformSampler::unitSphere(const Vector2& uv)
{
  float z = 1.0f - 2.0f * uv[0];
  float r = sqrtf(fmax(0.0f, 1.0f - z * z));
  float phi = TWO_PI * uv[1];
  return { r * cosf(phi), r * sinf(phi), z };
}

Vector3 UniformSampler::triangleBasuOwen(const float& u) // slower but has better variance
{
  uint32_t uFixedPoint = static_cast<uint32_t>(u * (1ULL << 32)); // u in range [0,1], 1 << 32 = 2^32 so ULL
  Vector2 A{ 1.0f, 0.0f }, B{ 0.0f, 1.0f }, C{ 0.0f, 0.0f };
  uint32_t digit;
  Vector2 An, Bn, Cn;
  for (uint32_t i = 0; i < 16; ++i)
  {
    // extract base4 digit
    digit = (uFixedPoint >> (2 * (15 - i))) & 3; // == (u / (15 - i)) % 4
    switch (digit)
    {
    case 0:
      An = (B + C) * 0.5f;
      Bn = (A + C) * 0.5f;
      Cn = (A + B) * 0.5f;
      break;
    case 1:
      An = A;
      Bn = (A + B) * 0.5f;
      Cn = (A + C) * 0.5f;
      break;
    case 2:
      An = (B + A) * 0.5f;
      Bn = B;
      Cn = (B + C) * 0.5f;
      break;
    case 3:
      An = (C + A) * 0.5f;
      Bn = (C + B) * 0.5f;
      Cn = C;
      break;
    }
    A = An;
    B = Bn;
    C = Cn;
  }
  An = (A + B + C) / 3.0f;
  // barycentric coordinates
  return { An[0], An[1], 1.0f - An[0] - An[1] };
}

Vector3 UniformSampler::triangle(const Vector2& uv)
{
  float su0 = sqrtf(uv[0]);
  float b0 = 1.0f - su0;
  float b1 = uv[1] - su0;
  return { b0, b1, 1.0f - b0 - b1 };
}