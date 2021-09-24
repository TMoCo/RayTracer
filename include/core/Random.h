//
// utility random number generation
//

#include <core/types.h>

#include <math/Vectors.h>

#include <random>

namespace Random {

    typedef std::uniform_real_distribution<F32> uniform_f;
    typedef std::uniform_int_distribution<I32>  uniform_i;
    typedef std::uniform_int_distribution<UI32> uniform_ui;

    // random float
    inline F32 UniformF32() {
        static uniform_f distribution(0.0f, 1.0f);
        static std::mt19937 generator;
        return distribution(generator);
    }

    inline F32 UniformF32(F32 min, F32 max) {
        static uniform_f distribution(min, max);
        static std::mt19937 generator;
        return distribution(generator);
    }

    // random int
    inline I32 UniformI32(I32 min, I32 max) {
        static uniform_i distribution(min, max);
        static std::mt19937 generator;
        return distribution(generator);
    }

    // random uv 
    inline Vector2 UniformUV() {
        static uniform_f distribution(0.0f, 1.0f);
        static std::mt19937 generator;
        return { distribution(generator), distribution(generator) };
    }
};
