//
// colour 
//

#ifndef COLOUR_H
#define COLOUR_H

#include <math/Vectors.h>

#include <cmath>

#define LUMINANCE {0.2126f, 0.7152f, 0.0722f}

typedef Vector3 colour;

namespace Colour {

    static colour Black{0.0f, 0.0f, 0.0f};
    static colour White{1.0f, 1.0f, 1.0f};
    static colour Red  {1.0f, 0.0f, 0.0f};
    static colour Green{0.0f, 1.0f, 0.0f};
    static colour Blue {0.0f, 0.0f, 1.0f};

    inline F32 Luminance(colour c) {
        return c.Dot(LUMINANCE);
    }

    inline colour GammaCorrection(colour c, F32 gamma) {
        F32 invGamma = 1.0f / gamma;
        return colour{std::pow(c.x, invGamma), std::pow(c.y, invGamma), std::pow(c.z, invGamma)};
    }

    // https://64.github.io/tonemapping/
    inline colour ReinhardTMO(colour c) {
        return c / (White + c);
    }

    inline colour ReinhardExtendedTMO(colour c, F32 maxWhite) {
        return (c * (White + (c / colour{maxWhite * maxWhite}))) / (White + c);
    }

};

#endif // !COLOUR_H