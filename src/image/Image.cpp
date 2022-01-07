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

Image::Image(int width, int height, int channels, const byte_t* initData)
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

byte_t* Image::operator[](int index)
{
  return data + (size_t)index * width;
}

const byte_t* Image::operator[](int index) const
{
  return data + (size_t)index * width;
}

void Image::clear()
{
  memset(data, 0, size() * (size_t)channels);
}

void Image::copy(const byte_t* inData)
{
  memcpy(data, inData, size() * channels);
}

bool Image::resize(int w, int h)
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

size_t Image::size() const
{
  return (size_t)width * height;
}

void Image::writePixelColour(int u, int v, const float* colourValues)
{
  byte_t* pixel = data + ((size_t)v * width + u) * channels;
  for (int i = 0; i < channels; ++i)
  {
    float colourValue = *(colourValues + i);
    *(pixel + i) = (byte_t)(sqrtf(colourValue / (colourValue + 1.0f)) * 255.0f); // tone mapping + gamma correction
  }
}

bool Image::writeToImageFile(const std::string& path) const
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

  return stbi_write_jpg(path.c_str(), width, height, channels, staging.data, 90) == 0 ? false : true; // could just return value
}

const byte_t* Image::getTexel(float u, float v) const
{
  int col = (int)(u * (width - 1));
  int row = (int)(v * (height - 1));
  return &data[(row * width + col) * channels];
}

void Image::allocate()
{
  data = allocAligned<byte_t>((size_t)width * height * channels);
  if (!data)
  {
    ERROR_MSG("Failed to allocate memory for buffer!\n");
    exit(-1); // big bad error
  }
}

void Image::release()
{
  if (data)
  {
    freeAligned(data);
    data = nullptr;
  }
}