//
// Image class definition
//

#include <image/Image.h>

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>

void Image::AllocatePixels() {
    pixels = (colour*)std::calloc(width * height, sizeof(colour));
    if (pixels == nullptr) {
        std::fprintf(stderr, "Could not allocate image!\n");
        std::exit(1);
    }
}

void Image::FreePixels() {
    if (pixels) {
        std::free(pixels);
        pixels = nullptr;
    }
}

Image::Image(UI64 w, UI64 h) : pixels(nullptr), width(w), height(h) {
    Resize(width, height);
}

Image::~Image() {
    FreePixels();
}

colour* Image::operator[](UI32 index) { 
    return pixels + (index * width);
}

const colour* Image::operator[](UI32 index) const {
    return pixels + (index * width);
}

void Image::Clear() {
    std::memset(pixels, 0, width * height * sizeof(colour));
}

bool Image::Resize(UI64 w, UI64 h) {
    width = w < MAX_IMAGE_DIMENSION ? w : MAX_IMAGE_DIMENSION;
    height = h < MAX_IMAGE_DIMENSION ? h : MAX_IMAGE_DIMENSION;

    if ((width == 0) || (height == 0))
        return false;

    FreePixels(); // guarantees that pixels == nullptr

    AllocatePixels();

    return true;
}

void Image::ToPPM(std::ostream& out) {
    out << "P3" << std::endl;
    out << "# PPM File" << std::endl;
    out << width << " " << height << std::endl;
    out << 255 << std::endl;
    
    colour pixel;
    // loop through pixels, reading them:
    for (UI64 row = height; row > 0; --row) { 
        for (UI64 col = 0; col < width; ++col) {
            if (col != 0) // put a space before each one except the first
                out << " ";
            pixel = pixels[(row - 1) * width + col][0] * 255.0f;
            out << (UI32) pixel[0] << " " << (UI32) pixel[1] << " " << (UI32) pixel[2] << std::endl;
        } 
        out << std::endl;
    }
}