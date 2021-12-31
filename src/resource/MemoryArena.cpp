/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 30/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <core/core.h>
#include <resource/MemoryArena.h>

MemoryArena::MemoryArena(size_t blockSize)
  : blockSize(blockSize)
{

}

MemoryArena::~MemoryArena()
{
  freeAligned(currentBlock);
  
  for (auto& block : usedBlocks)
  {
    freeAligned(block.second);
  }

  for (auto& block : availableBlocks)
  {
    freeAligned(block.second);
  }
}

void* MemoryArena::alloc(size_t nBytes)
{
  nBytes = ((nBytes + 15) & (~15)); // round up to closest multiple of 16

  if (currentBlockPos + nBytes > currentAllocSize) // need more memory
  {
    if (currentBlock) // mark block as used
    {
      usedBlocks.push_back(std::make_pair(currentAllocSize, currentBlock));
      currentBlock = nullptr;
    }

    for (auto block = availableBlocks.begin(); block != availableBlocks.end(); ++block)
    {
      // if available block can fit requested bytes, use block
      if (block->first >= nBytes)
      {
        currentAllocSize = block->first;
        currentBlock = block->second;
        availableBlocks.erase(block);
        break;
      }
    }

    if (!currentBlock) // no valid block, allocate new one
    {
      currentAllocSize = std::max(nBytes, blockSize);
      currentBlock = allocAligned<byte>(currentAllocSize);
    }

    currentBlockPos = 0;
  }

  void* ret = currentBlock + currentBlockPos;
  currentBlockPos += nBytes;
  return ret;
}

void MemoryArena::reset()
{
  currentBlockPos = 0;
  availableBlocks.splice(availableBlocks.begin(), usedBlocks); // put all used blocks in available blocks
}

size_t MemoryArena::totalAllocated() const
{
  size_t total = currentAllocSize;

  for (const auto& alloc : usedBlocks)
  {
    total += alloc.first;
  }

  for (const auto& alloc : availableBlocks)
  {
    total += alloc.first;
  }

  return total;
}