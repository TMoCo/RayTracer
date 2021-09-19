//
// Image class
//

#ifndef IMAGE_H
#define IMAGE_H 1

#include <core/types.h>

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

    T* operator[](UI32 index);
    const T* operator[](UI32 index) const;

    void Clear();
    bool Resize(UI64 w, UI64 h);
};


template<typename T>
void Image<T>::AllocatePixels() {
    std::cout << width << " " << height << " " << sizeof(T) << "\n";
    pixels = (T*)calloc(width * height, sizeof(T));
    if (pixels == nullptr) {
        std::fprintf(stderr, "Could not allocate image!\n");
        std::exit(1);
    }
}

template<typename T>
void Image<T>::FreePixels() {
    if (pixels) {
        free(pixels);
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

#endif