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
  typedef std::uniform_real_distribution<float> uniform_f;
  typedef std::uniform_int_distribution<int>  uniform_i;
  typedef std::uniform_int_distribution<uint32_t> uniform_ui;

  static std::random_device device;
  static std::mt19937 generator(device());
  static uniform_f distribution01(0.0f, 1.0f);

  inline float uniformfloat() 
  {
    return distribution01(generator);
  }

  inline float uniformfloat(float min, float max) 
  {
    static uniform_f distribution(min, max);
    return distribution(generator);
  }

  inline int uniformint(int min, int max) 
  {
    static uniform_i distribution(min, max);
    return distribution(generator);
  }

  inline uint32_t uniformuint32_t(uint32_t min, uint32_t max)
  {
    static uniform_ui distribution(min, max);
    return distribution(generator);
  }

  inline Vector2 uniformUV() 
  {
    return { distribution01(generator), distribution01(generator) };
  }

  inline Vector3 uniformVector3(float min, float max)
  {
    return { uniformfloat(min, max), uniformfloat(min, max), uniformfloat(min, max) };
  }
};

#endif // !RANDOM_H