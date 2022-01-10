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

#define MAX_IMG_SIZE 4000 // max 4k image
#define MIN_IMG_SIZE 50

class Image 
{
public:
  Image();

  Image(int width, int height, int channels, const byte_t* initData = nullptr);
  
  ~Image();

  inline const int getWidth() const { return width; }
  
  inline const int getHeight() const { return height; }

  inline const int getChannels() const { return channels; }

  byte_t* operator[](int index);
  
  const byte_t* operator[](int index) const;

  void clear();

  void copy(const byte_t* data);

  bool resize(int w, int h);

  size_t size() const;

  void writePixelAt(size_t offset, const float* colour);

  void writePixelAt(int u, int v, const float* colourValues);

  void writeBlockAt(size_t blockSize, void* inData, int startU = 0, int startV = 0);

  void writeBlockAt(size_t blockSize, void* inData, size_t offset = 0);

  bool writeToImageFile(const std::string& path) const;

  const byte_t* getTexel(float u, float v) const;

private:
  byte_t* data;

  int width, height, channels;

  void allocate();

  void release();

};

#endif // !IMAGE_H