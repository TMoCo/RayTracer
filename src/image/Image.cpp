/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 20/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Image class definition 
//

#include <core/core.h>
#include <core/debug.h>
#include <image/Image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

Image::Image()
  : data{ nullptr }, width{ 0 }, height{ 0 }, channels{ 0 }
{ }

Image::Image(UI32 width, UI32 height, UI32 channels, const byte* initData)
  : data{ nullptr }, width{ width }, height{ height }, channels{ channels }
{
  resize(width, height);
  if (initData)
  {
    copy(initData);
  }
}

Image::~Image()
{
  release();
}

byte* Image::operator[](UI32 index)
{
  return data + (size_t)(index * width);
}

const byte* Image::operator[](UI32 index) const
{
  return data + (size_t)(index * width);
}

void Image::clear()
{
  memset(data, 0, (size_t)(size() * channels));
}

void Image::copy(const byte* inData)
{
  memcpy(data, inData, (size_t)(size() * channels));
}

bool Image::resize(UI32 w, UI32 h)
{
  if ((width == 0) || (height == 0) || (channels == 0))
  {
    return false;
  }

  width = std::min(w, MAX_IMAGE_SIZE);
  height = std::min(h, MAX_IMAGE_SIZE);

  release(); // guarantees that pixels == nullptr

  allocate();

  return true;
}

UI32 Image::size() const
{
  return width * height;
}

void Image::writePixelColour(UI32 u, UI32 v, const F32* colourValues)
{
  byte* pixel = data + (size_t)(v * width + u) * channels;
  for (UI32 i = 0; i < channels; ++i)
  {
    F32 colourValue = *(colourValues + i);
    *(pixel + i) = (byte)(sqrtf(colourValue / (colourValue + 1.0f)) * 255.0f); // tone mapping + gamma correction
  }
}

bool Image::writeToImageFile(const std::string& path) const
{
  Image staging{ width, height, channels };

  byte* pStaging = staging.data;
  // flip image horizontally:
  // stb expects image pixels stored from top to bottom, left to right
  for (I32 col = height - 1; col >= 0; --col)
  {
    for (I32 row = 0; row < (I32)width; row++)
    {
      for (UI32 channel = 0; channel < channels; channel++)
      {
        *(pStaging + channel) = data[(col * width + row) * channels + channel];
      }
      pStaging += channels;
    }
  }

  return stbi_write_jpg(path.c_str(), width, height, channels, staging.data, 90) != 0;
}

const byte* Image::getTexel(F32 u, F32 v) const
{
  I32 col = (I32)(u * (width - 1));
  I32 row = (I32)(v * (height - 1));
  return &data[(row * width + col) * channels];
}

void Image::allocate()
{
  data = (byte*)calloc(size(), channels);
  if (!data)
  {
    ERROR_MSG("Failed to allocate memory for buffer!\n");
    exit(-1);
  }
}

void Image::release()
{
  if (data)
  {
    free(data);
    data = nullptr;
  }
}