//
// Image class
//

#ifndef IMAGE_H
#define IMAGE_H 1

#include <core/types.h>

#include <image/Colour.h>

#include <iostream>

#define MAX_IMAGE_DIMENSION 4000UL // max 4k image

class Image {
private:
    colour* pixels;

    UI64 width, height;

    void AllocatePixels();
    void FreePixels();

public:

    Image(UI64 w = 0, UI64 h = 0);

    ~Image();

    inline UI64 Width() { return width; }
    inline UI64 Height() { return height; }

    void ToPPM(std::ostream& out);
    

    colour* operator[](UI32 index);
    const colour* operator[](UI32 index) const;

    void Clear();
    bool Resize(UI64 w, UI64 h);
};

#endif