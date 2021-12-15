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

#include <assert.h>

constexpr F32 radians(F32 x)
{
  return x * PI_180;
}

constexpr F32 clamp(F32 val, F32 min, F32 max)
{
  assert(min < max);
  return (val > max ? max : val) < min ? min : val;
}

constexpr bool quadratic(F32 a, F32 b, F32 c, F32* t0, F32* t1)
{
  F64 d = (F64)b * (F64)b - 4.0 * (F64)a * (F64)c;
  
  if (d < 0.0)
  {
    return false;
  }
  
  F64 q = b < 0.0 ? -0.5 * ((F64)b - sqrt(d)) : -0.5 * ((F64)b + sqrt(d));
  
  *t0 = (F32)(q / a);
  *t1 = (F32)(c / q);
  
  if (*t1 > *t0)
  {
    std::swap(*t0, *t1);
  }

  return true;
}

#endif // !ROUTINES_H
