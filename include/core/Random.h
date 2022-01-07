/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// utility for random number generation
//

#ifndef RANDOM_H
#define RANDOM_H 1

#include <Vector3.h>
#include <Vector2.h>
#include <random>

namespace random {
  typedef std::uniform_int_distribution<int> udi_t;
  typedef std::uniform_real_distribution<float> udf_t;
  typedef std::uniform_int_distribution<uint32_t> udui_t;

  static std::random_device device;
  static std::mt19937 generator(device());

  // uniform float in range [0,1]
  static udf_t udf_0_1(0.0f, 1.0f);
  // uniform float in range [-1,1]
  static udf_t udf_m1_1(-1.0f, 1.0f);

  inline float uniform_float(float min, float max) 
  {
    static udf_t distribution(min, max);
    return distribution(generator);
  }

  inline int uniform_int(int min, int max) 
  {
    static udi_t distribution(min, max);
    return distribution(generator);
  }

  inline uint32_t uniform_uint32_t(uint32_t min, uint32_t max)
  {
    static udui_t distribution(min, max);
    return distribution(generator);
  }

  inline Vector2 uniformUV() 
  {
    return { udf_0_1(generator), udf_0_1(generator) };
  }
};

#endif // !RANDOM_H