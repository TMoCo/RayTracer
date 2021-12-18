/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Mathematical routines
//

#ifndef ROUTINES_H
#define ROUTINES_H 1

#include <core/types.h>
#include <math/constants.h>
#include <intrin.h>

#include <assert.h>

constexpr inline F32 radians(F32 x)
{
  return x * PI_180;
}


constexpr inline F32 clamp(F32 val, F32 min, F32 max)
{
  assert(min < max);
  return (val > max ? max : val) < min ? min : val;
}

constexpr inline bool quadratic(F32 a, F32 b, F32 c, F32* t0, F32* t1)
{
  F64 d = (F64)b * (F64)b - 4.0 * (F64)a * (F64)c;
  
  if (d < 0.0)
  {
    return false;
  }
  
  F64 rootDelta = sqrt(d);
  F64 q = b < 0.0 ? -0.5 * ((F64)b - rootDelta) : -0.5 * ((F64)b + rootDelta);
  
  *t0 = (F32)(q / a);
  *t1 = (F32)(c / q);
  
  if (*t0 > *t1)
  {
    std::swap(*t0, *t1);
  }

  return true;
}

constexpr inline F32 inverseSquareRoot(F32 x)
{
  UI32 *i = (UI32*)&x;
  *i = 0x5F375A86 - (*i >> 1);
  return x * (1.5f - 0.5f * x * x * x);
}

#endif // !ROUTINES_H
