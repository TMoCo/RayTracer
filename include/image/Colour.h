/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Colour 
//

#ifndef COLOUR_H
#define COLOUR_H 1

#include <Vector3.h>


#define LUMINANCE { 0.2126f, 0.7152f, 0.0722f }

typedef Vector3 Colour;

namespace colour
{
  static Colour Black{ 0.0f, 0.0f, 0.0f };
  static Colour White{ 1.0f, 1.0f, 1.0f };
  static Colour Red{ 1.0f, 0.0f, 0.0f };
  static Colour Green{ 0.0f, 1.0f, 0.0f };
  static Colour Blue{ 0.0f, 0.0f, 1.0f };

  inline float luminance(Colour c)
  {
    return c.dot(LUMINANCE);
  }

  inline Colour gammaCorrection(Colour c, float gamma)
  {
    float invGamma = 1.0f / gamma;
    return { powf(c[0], invGamma), powf(c[1], invGamma), powf(c[2], invGamma) };
  }

  // https://64.github.io/tonemapping/
  inline Colour reinhardTMO(Colour c)
  {
    return c / (White + c);
  }

  inline Colour reinhardExtendedTMO(Colour c, float maxWhite)
  {
    return (c * (White + (c / Colour{ maxWhite * maxWhite }))) / (White + c);
  }

};

#endif // !COLOUR_H
