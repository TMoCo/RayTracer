//
// Image class
//

#ifndef IMAGE_H_
#define IMAGE_H_ 1

#include <core/types.h>
#include <core/debug.h>

#define MAX_BUFFER_SIZE 4000UL // max 4k image

template <typename T>
class buffer {
private:
  T* data;
  
  UI64 width, height;

  void allocate();
  
  void free();

public:

  buffer(UI64 w = 0, UI64 h = 0);
  
  ~buffer();

  inline UI64 getWidth() { return width; }
  
  inline UI64 getHeight() { return height; }

  T* operator[](UI32 index);
  
  const T* operator[](UI32 index) const;

  void clear();

  bool resize(UI64 w, UI64 h);
};


template <typename T>
void buffer<T>::allocate()
{
  data = (T*)std::calloc(width * height, sizeof(T));
  if (data == nullptr)
  {
    DEBUG_PRINT("Could not allocate image!\n");
    std::exit(1);
  }
}

template <typename T>
void buffer<T>::free()
{
  if (data)
  {
    std::free(data);
    data = nullptr;
  }
}

template <typename T>
buffer<T>::buffer(UI64 w, UI64 h) : data(nullptr), width(w), height(h)
{
  resize(width, height);
}

template <typename T>
buffer<T>::~buffer()
{
  free();
}

template <typename T>
T* buffer<T>::operator[](UI32 index)
{
  return data + (index * height);
}

template <typename T>
const T* buffer<T>::operator[](UI32 index) const
{
  return data + (index * height);
}

template <typename T>
void buffer<T>::clear()
{
  memset(data, 0, width * height * sizeof(T));
}

template <typename T>
bool buffer<T>::resize(UI64 w, UI64 h)
{
  width = w < MAX_BUFFER_SIZE ? w : MAX_BUFFER_SIZE;
  height = h < MAX_BUFFER_SIZE ? h : MAX_BUFFER_SIZE;

  if ((width == 0) || (height == 0))
    return false;

  free(); // guarantees that pixels == nullptr

  allocate();

  return true;
}


#endif