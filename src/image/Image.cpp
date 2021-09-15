//
// Image class definition
//

#include <image/Image.h>

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>

template<typename T>
void Image<T>::AllocatePixels() {
    pixels = (T*)calloc(width * height, sizeof(T));

    if (pixels == nullptr) {
        std::fprintf(stderr, "Could not allocate image!");
        exit(1);
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
Image<T>::Image(uint64_t w, uint64_t h) : pixels(nullptr), width(w), height(h) {
    Resize(width, height);
}

template<typename T>
Image<T>::~Image() {
    FreePixels();
}

template<typename T>
T* Image<T>::operator[](uint32_t index) { 
    return pixels + (index * width);
}

template<typename T>
const T* Image<T>::operator[](uint32_t index) const {
    return pixels + (index * width);
}

template<typename T>
void Image<T>::Clear() {
    std::memset(pixels, 0, width * height * sizeof(T));
}

template<typename T>
bool Image<T>::Resize(uint64_t w, uint64_t h) {
    width = w < MAX_IMAGE_DIMENSION ? w : MAX_IMAGE_DIMENSION;
    height = h < MAX_IMAGE_DIMENSION ? h : MAX_IMAGE_DIMENSION;

    if ((width == 0) || (height == 0))
        return false;

    FreePixels(); // guarantees that pixels == nullptr

    AllocatePixels(); 

    return true;
}