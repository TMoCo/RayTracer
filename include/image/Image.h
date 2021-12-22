/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 20/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Image class declaration
//

#ifndef IMAGE_H
#define IMAGE_H 1

#include <core/types.h>

#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MAX_IMAGE_SIZE 4000U // max 4k image

class Image 
{
public:
  Image();

  Image(UI32 width, UI32 height, UI32 channels, const byte* initData = nullptr);
  
  ~Image();

  inline const UI32 getWidth() const { return width; }
  
  inline const UI32 getHeight() const { return height; }

  inline const UI32 getChannels() const { return channels; }

  byte* operator[](UI32 index);
  
  const byte* operator[](UI32 index) const;

  void clear();

  void copy(const byte* data);

  bool resize(UI32 w, UI32 h);

  UI32 size() const;

  void writePixelColour(UI32 u, UI32 v, const F32* colourValues);

  bool writeToImageFile(const std::string& path) const;

  const byte* getTexel(F32 u, F32 v) const;

private:
  byte* data;

  UI32 width, height, channels;

  void allocate();

  void release();

};

#endif // !IMAGE_H