//
// Image class declaration
//

#ifndef IMAGE_H
#define IMAGE_H 1

#include <cstdint>
#include <sys/types.h>

#define MAX_IMAGE_DIMENSION 4000UL // max 4k image

template<typename T>
union rgba {
    T channels[4];
    struct {
        T r, g, b, a;
    };

    inline T& operator[](uint32_t index) { return channels[index]; }
    inline const T& operator[](uint32_t index) const { return channels[index]; }
};

typedef rgba<float> rgba_f32;
typedef rgba<uint32_t> rgba_ui32;
typedef rgba<u_char> rgba_b;

template<typename T>
class Image {
private:
    T* pixels;
    uint64_t width, height;

    void AllocatePixels();
    void FreePixels();

public:
    Image(uint64_t w = 0, uint64_t h = 0);

    ~Image();

    T* operator[](uint32_t index);
    const T* operator[](uint32_t index) const;

    void Clear();
    bool Resize(uint64_t w, uint64_t h);
};

#endif