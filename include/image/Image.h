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

#include <stb_image.h>
#include <stb_image_write.h>

#define MAX_IMG_SIZE 4000 // max 4k image
#define MIN_IMG_SIZE 50

template<typename T>
class Image 
{
public:
  inline Image();

  inline Image(int width, int height, int channels, const T* initData = nullptr);
  
  inline ~Image();

  inline const int getWidth() const { return width; }
  
  inline const int getHeight() const { return height; }

  inline const int getChannels() const { return channels; }

  inline  T* operator[](int index);
  
  inline const T* operator[](int index) const;

  inline void clear();

  inline void copy(const T* data);

  inline bool resize(int w, int h);

  inline size_t size() const;

  inline void writePixelAt(size_t offset, const float* colour);

  inline void writePixelAt(int u, int v, const float* colourValues);

  inline void writeBlockAt(size_t blockSize, void* inData, int startU = 0, int startV = 0);

  inline void writeBlockAt(size_t blockSize, void* inData, size_t offset = 0);

  inline bool writeToImageFile(const std::string& path) const;

  inline const T* getTexel(float u, float v) const;

private:
  T* data;

  int width, height, channels;

  void allocate();

  void release();

};

template<typename T>
Image<T>::Image()
  : data{ nullptr }, width{ 0 }, height{ 0 }, channels{ 0 }
{ }

template<typename T>
Image<T>::Image(int width, int height, int channels, const T* initData)
  : data{ nullptr }, width{ width }, height{ height }, channels{ channels }
{
  resize(width, height);
  if (initData)
  {
    copy(initData);
  }
}

template<typename T>
Image<T>::~Image()
{
  release();
}

template<typename T>
T* Image<T>::operator[](int index)
{
  return data + (size_t)index * width;
}

template<typename T>
const T* Image<T>::operator[](int index) const
{
  return data + (size_t)index * width;
}

template<typename T>
void Image<T>::clear()
{
  memset(data, 0, size());
}

template<typename T>
void Image<T>::copy(const T* inData)
{
  memcpy(data, inData, size());
}

template<typename T>
bool Image<T>::resize(int w, int h)
{
  if (w < 1 || h < 1 || channels < 1)
  {
    return false;
  }

  width = w;
  height = h;

  release(); // guarantees that pixels == nullptr

  allocate();

  return true;
}

template<typename T>
size_t Image<T>::size() const
{
  return (size_t)width * height * channels;
}

template<typename T>
void Image<T>::writePixelAt(size_t offset, const float* colour)
{
  T* pixel = data + (offset * channels);
  for (int i = 0; i < channels; ++i)
  {
    float c = *(colour + i);
    *(pixel + i) = (T)(sqrtf(c / (c + 1.0f)) * 255.0f);
  }
}

template<typename T>
void Image<T>::writePixelAt(int u, int v, const float* colour)
{
  T* pixel = data + ((size_t)v * width + u) * channels;
  for (int i = 0; i < channels; ++i)
  {
    float c = *(colour + i);
    *(pixel + i) = (T)(sqrtf(c / (c + 1.0f)) * 255.0f); // tone mapping + gamma correction
  }
}

template<typename T>
void Image<T>::writeBlockAt(size_t blockSize, void* indata, int startU, int startV)
{
  size_t offset = (startV * height + startU) * channels;
  DEBUG_ASSERT(offset + blockSize <= size(), "Trying to write more than image size");
  memcpy(data + offset, indata, blockSize);
}

template<typename T>
void Image<T>::writeBlockAt(size_t blockSize, void* inData, size_t offset)
{
  DEBUG_ASSERT(offset + blockSize <= size(), "Trying to write more than image size");
  memcpy(data + offset, inData, blockSize);
}

template<typename T>
bool Image<T>::writeToImageFile(const std::string& path) const
{
  Image staging{ width, height, channels };

  byte_t* pStaging = staging.data;
  // flip image horizontally:
  // stb expects image pixels stored from top to bottom, left to right
  for (int col = height - 1; col >= 0; --col)
  {
    for (int row = 0; row < width; row++)
    {
      for (int channel = 0; channel < channels; channel++)
      {
        *(pStaging + channel) = data[(col * width + row) * channels + channel];
      }
      pStaging += channels;
    }
  }

  return stbi_write_jpg(path.c_str(), width, height, channels, staging.data, 90);

}

template<typename T>
const T* Image<T>::getTexel(float u, float v) const
{
  int col = (int)(u * (width - 1));
  int row = (int)(v * (height - 1));
  return &data[(row * width + col) * channels];
}

template<typename T>
void Image<T>::allocate()
{
  data = allocAligned<T>((size_t)width * height * channels);
  if (!data)
  {
    ERROR_MSG("Failed to allocate memory for buffer!\n");
    exit(-1); // big bad error
  }
}

template<typename T>
void Image<T>::release()
{
  if (data)
  {
    freeAligned(data);
    data = nullptr;
  }
}


#endif // !IMAGE_H