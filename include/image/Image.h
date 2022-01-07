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

#include <core/core.h>
#include <string>

#define MAX_IMAGE_SIZE 4000U // max 4k image

class Image 
{
public:
  Image();

  Image(uint32_t width, uint32_t height, uint32_t channels, const byte_t* initData = nullptr);
  
  ~Image();

  inline const uint32_t getWidth() const { return width; }
  
  inline const uint32_t getHeight() const { return height; }

  inline const uint32_t getChannels() const { return channels; }

  byte_t* operator[](uint32_t index);
  
  const byte_t* operator[](uint32_t index) const;

  void clear();

  void copy(const byte_t* data);

  bool resize(uint32_t w, uint32_t h);

  uint32_t size() const;

  void writePixelColour(uint32_t u, uint32_t v, const float* colourValues);

  bool writeToImageFile(const std::string& path) const;

  const byte_t* getTexel(float u, float v) const;

private:
  byte_t* data;

  uint32_t width, height, channels;

  void allocate();

  void release();

};

#endif // !IMAGE_H