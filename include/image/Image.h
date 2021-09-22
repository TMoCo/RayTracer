//
// Image class
//

#ifndef IMAGE_H
#define IMAGE_H 1

#include <core/types.h>

#include <math/Vector4.h>

#include <sys/types.h>
#include <iostream>

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>

#define MAX_IMAGE_DIMENSION 4000UL // max 4k image

template<typename T>
union rgba {
    T channels[4];
    struct {
        T r, g, b, a;
    };

    inline rgba<T> operator =(const T* c) { 
        std::memcpy(channels, c, 4 * sizeof(T)); 
        return *this; 
    }

    inline T& operator[](uint32_t index) { return channels[index]; }
    inline const T& operator[](uint32_t index) const { return channels[index]; }
};

typedef rgba<F32> rgba_f; // single
typedef rgba<UI32> rgba_ui; // unsigned int
typedef rgba<byte> rgba_b; // byte

template<typename T>
class Image {
private:
    T* pixels;

    uint64_t width, height;

    void AllocatePixels();
    void FreePixels();

public:

    Image(UI64 w = 0, UI64 h = 0);

    ~Image();

    inline UI64 Width() { return width; }
    inline UI64 Height() { return height; }

    void ToPPM(std::ostream& out);
    
    rgba_b ToByte(T pixel);

    T* operator[](UI32 index);
    const T* operator[](UI32 index) const;

    void Clear();
    bool Resize(UI64 w, UI64 h);
};


template<typename T>
void Image<T>::AllocatePixels() {
    pixels = (T*)std::calloc(width * height, sizeof(T));
    if (pixels == nullptr) {
        std::fprintf(stderr, "Could not allocate image!\n");
        std::exit(1);
    }
}

template<typename T>
void Image<T>::FreePixels() {
    if (pixels) {
        std::free(pixels);
        pixels = nullptr;
    }
}

template<typename T>
Image<T>::Image(UI64 w, UI64 h) : pixels(nullptr), width(w), height(h) {
    Resize(width, height);
}

template<typename T>
Image<T>::~Image() {
    FreePixels();
}

template<typename T>
T* Image<T>::operator[](UI32 index) { 
    return pixels + (index * width);
}

template<typename T>
const T* Image<T>::operator[](UI32 index) const {
    return pixels + (index * width);
}

template<typename T>
void Image<T>::Clear() {
    std::memset(pixels, 0, width * height * sizeof(T));
}

template<typename T>
bool Image<T>::Resize(UI64 w, UI64 h) {
    width = w < MAX_IMAGE_DIMENSION ? w : MAX_IMAGE_DIMENSION;
    height = h < MAX_IMAGE_DIMENSION ? h : MAX_IMAGE_DIMENSION;

    if ((width == 0) || (height == 0))
        return false;

    FreePixels(); // guarantees that pixels == nullptr

    AllocatePixels();

    return true;
}

template<typename T>
rgba_b Image<T>::ToByte(T pixel) {
    rgba_b bytes;
    if (typeid(T) == typeid(UI32)) {
        bytes = {(byte)(pixel.r * 255) , (byte)(pixel.g * 255), (byte)(pixel.b * 255), (byte)(pixel.a * 255)};
    }
    if (typeid(T) == typeid(F32)) {
        bytes = {(byte)(pixel.r * 255.0f) , (byte)(pixel.g * 255.0f), (byte)(pixel.b * 255.0f), (byte)(pixel.a * 255.0f)};
    }
    return bytes;
}

template<typename T>
void Image<T>::ToPPM(std::ostream& out) {
    out << "P3" << std::endl;
    out << "# PPM File" << std::endl;
    out << width << " " << height << std::endl;
    out << 255 << std::endl;
    
    Vector3 colour;
    // loop through pixels, reading them:
    for (UI64 row = height; row > 0; --row) { 
        for (UI64 col = 0; col < width; ++col) {
            if (col != 0) // put a space before each one except the first
                out << " ";
            colour = &pixels[(row - 1) * width + col][0];
            colour *= 255.0f;
            out << (UI32) colour[0] << " " << (UI32) colour[1] << " " << (UI32) colour[2] << std::endl;
        } 
        out << std::endl;
    }
}

#endif