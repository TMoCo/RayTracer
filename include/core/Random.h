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
  typedef std::uniform_int_distribution<int> ud_int_t;
  typedef std::uniform_real_distribution<float> ud_float_t;
  typedef std::uniform_int_distribution<uint32_t> ud_uint_t;

  static std::random_device device;
  static std::mt19937 generator(device());

  static ud_float_t ud_float_0_1(0.0f, 1.0f);
  static ud_float_t ud_float_m1_1(-1.0f, 1.0f);

  inline float uniform_float(float min, float max) 
  {
    static ud_float_t distribution(min, max);
    return distribution(generator);
  }

  inline int uniform_int(int min, int max) 
  {
    static ud_int_t distribution(min, max);
    return distribution(generator);
  }

  inline uint32_t uniform_uint32_t(uint32_t min, uint32_t max)
  {
    static ud_uint_t distribution(min, max);
    return distribution(generator);
  }

  inline Vector2 uniform_uv() 
  {
    return { ud_float_0_1(generator), ud_float_0_1(generator) };
  }
};

#endif // !RANDOM_H