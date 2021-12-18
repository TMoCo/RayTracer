/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// utility for random number generation
//

#include <core/types.h>

#include <math/thomath.h>

#include <random>

namespace random {
  typedef std::uniform_real_distribution<F32> uniform_f;
  typedef std::uniform_int_distribution<I32>  uniform_i;
  typedef std::uniform_int_distribution<UI32> uniform_ui;

  static std::mt19937 generator;

  inline F32 uniformF32() 
  {
    static uniform_f distribution(0.0f, 1.0f);
    return distribution(generator);
  }

  inline F32 uniformF32(F32 min, F32 max) 
  {
    static uniform_f distribution(min, max);
    return distribution(generator);
  }

  inline I32 uniformI32(I32 min, I32 max) 
  {
    static uniform_i distribution(min, max);
    return distribution(generator);
  }

  inline UI32 uniformUI32(UI32 min, UI32 max)
  {
    static uniform_ui distribution(min, max);
    return distribution(generator);
  }

  inline Vector2 uniformUV() 
  {
    static uniform_f distribution(0.0f, 1.0f);
    return { distribution(generator), distribution(generator) };
  }

  inline Vector3 uniformVector3(F32 min, F32 max)
  {
    return { uniformF32(min, max), uniformF32(min, max), uniformF32(min, max) };
  }
};
