/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 30/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Memory arena
// https://www.pbr-book.org/3ed-2018/Utilities/Memory_Management
//

#ifndef MEMORY_ARENA_H
#define MEMORY_ARENA_H 1

#include <core/types.h>

#include <list>

class MemoryArena
{
public:
  MemoryArena(size_t blockSize = 262144);

  ~MemoryArena();

  void* alloc(size_t nBytes);

  template<typename T> 
  T* alloc(size_t n = 1, bool runConstructor = true);

  void reset();

  size_t totalAllocated() const;

private:
  const size_t blockSize;

  size_t currentBlockPos = 0, currentAllocSize = 0;

  byte* currentBlock = nullptr;

  std::list<std::pair<size_t, byte*>> usedBlocks, availableBlocks;
};


template<typename T>
T* MemoryArena::alloc(size_t n, bool construct)
{
  T* ret = (T*)alloc(n * sizeof(T));

  if (construct)
  {
    for (size_t i = 0; i < n; ++i)
    {
      new (&ret[i]) T();
    }
  }

  return ret;
}

#endif // !MEMORY_ARENA_H

