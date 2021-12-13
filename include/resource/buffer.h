/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// Image class
//

#ifndef IMAGE_H_
#define IMAGE_H_ 1

#include <core/types.h>
#include <core/debug.h>

#include <math.h>

#define MAX_BUFFER_SIZE 4000UL // max 4k image

template <typename T>
class buffer {
private:
  T* data;
  
  UI64 width, height, depth;

  void allocate();
  
  void release();

public:
  buffer(UI64 w = 0, UI64 h = 0, UI64 d = 1);
  
  ~buffer();

  inline UI64 getWidth() { return width; }
  
  inline UI64 getHeight() { return height; }

  inline UI64 getDepth() { return depth; }

  T* operator[](UI32 index);
  
  const T* operator[](UI32 index) const;

  void clear();

  bool resize(UI64 w, UI64 h, UI64 d);
};


template <typename T>
void buffer<T>::allocate()
{
  data = (T*)std::calloc(width * height * depth, sizeof(T));
  if (data == nullptr)
  {
    DEBUG_PRINT("Could not allocate image!\n");
    std::exit(1);
  }
}

template <typename T>
void buffer<T>::release()
{
  if (data)
  {
    std::free(data);
    data = nullptr;
  }
}

template <typename T>
buffer<T>::buffer(UI64 w, UI64 h, UI64 d) : data(nullptr), width(w), height(h), depth(d)
{
  resize(width, height, depth);
}

template <typename T>
buffer<T>::~buffer()
{
  release();
}

template <typename T>
T* buffer<T>::operator[](UI32 index)
{
  return data + (index * width);
}

template <typename T>
const T* buffer<T>::operator[](UI32 index) const
{
  return data + (index * width);
}

template <typename T>
void buffer<T>::clear()
{
  std::memset(data, 0, width * height * depth * sizeof(T));
}

template <typename T>
bool buffer<T>::resize(UI64 w, UI64 h, UI64 d)
{
  width  = w < MAX_BUFFER_SIZE ? w : MAX_BUFFER_SIZE;
  height = h < MAX_BUFFER_SIZE ? h: MAX_BUFFER_SIZE;
  depth  = d < 6 ? d : 6;

  if ((width == 0) || (height == 0))
    return false;

  release(); // guarantees that pixels == nullptr

  allocate();

  return true;
}
#endif