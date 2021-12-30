#include <render/bounds/LinearBVH.h>

LinearBVH::LinearBVH()
{

}

LinearBVH::LinearBVH(const std::vector<Shape*>& sceneShapes)
  : sceneShapes{ sceneShapes }
{
  if (sceneShapes.empty())
  {
    return;
  }

  std::vector<Shape*> orderedSceneShapes;

  AABB bounds; // bounds of all shape centroids

  std::vector<ShapePrimitiveInfo> shapePrimitiveInfo(sceneShapes.size());

  for (UI32 p = 0; p < shapePrimitiveInfo.size(); ++p)
  {
    shapePrimitiveInfo[p] = { p, sceneShapes[p]->getAABB() };
    bounds.mergeWithPoint(shapePrimitiveInfo[p].centroid);
  }

  std::vector<MortonPrimitive> mortonPrimitives(sceneShapes.size());
  I32 mortonBits = 10; // 10 bits per axis
  I32 mortonScale = 1 << mortonBits;
  
  for (UI32 mp = 0; mp < mortonPrimitives.size(); ++mp)
  {
    mortonPrimitives[mp].shapeIndex = shapePrimitiveInfo[mp].num;
    mortonPrimitives[mp].mortonCode = 
      apply3DMortonEncoding(bounds.getOffset(shapePrimitiveInfo[mp].centroid) * mortonScale);
  }

  radixSortMortonPrimitives(&mortonPrimitives);
}

UI32 LinearBVH::apply3DMortonEncoding(const Vector3& point)
{
  return (shift10BitsLeftToEvery3rd(point[0])) | (shift10BitsLeftToEvery3rd(point[1]) << 1) 
    | (shift10BitsLeftToEvery3rd(point[2]) << 2);
  // zyxzyxzyxzyxzyx ... 
}

// place the ith bit in the 3ith position (i < 10)
UI32 LinearBVH::shift10BitsLeftToEvery3rd(UI32 x)
{
  if (x == (1 << 10))
  {
    --x;
  }

  x = (x | (x << 16)) & 0b00000011000000000000000011111111;
  x = (x | (x << 8)) & 0b00000011000000001111000000001111;
  x = (x | (x << 4)) & 0b00000011000011000011000011000011;
  x = (x | (x << 2)) & 0b00001001001001001001001001001001;

  return x;
}

void LinearBVH::radixSortMortonPrimitives(std::vector<MortonPrimitive>* mortonPrimitives)
{
  std::vector<MortonPrimitive> tmp(mortonPrimitives->size());
  
  constexpr I32 bitsPerPass = 6;
  I32 nBits = 30;
  I32 nPasses = nBits / bitsPerPass;
  
  for (UI32 pass = 0; pass < nPasses; ++pass)
  {
    I32 lowBit = pass * bitsPerPass; // sort nBitsPerPass starting at lowBit

    std::vector<MortonPrimitive>& in = (pass & 1) ? tmp : *mortonPrimitives;
    std::vector<MortonPrimitive>& out = (pass & 1) ? *mortonPrimitives : tmp;

    constexpr I32 nBuckets = 1 << bitsPerPass;
    I32 bucketCount[nBuckets] = { 0 };

    constexpr I32 bitMask = nBuckets - 1;
    for (const MortonPrimitive& mp : in)
    {
      I32 bucket = (mp.mortonCode >> lowBit) & bitMask;
      ++bucketCount[bucket]; // count nb of values in each bucket
    }

    I32 outIndex[nBuckets]; // offsets into output array where bucket starts
    outIndex[0] = 0;
    for (UI32 i = 1; i < nBuckets; ++i)
    {
      outIndex[i] = outIndex[i - 1] + bucketCount[i - 1];
    }

    // recompute bucket primitives land in and store in output array
    for (const MortonPrimitive& mp : in)
    {
      I32 bucket = (mp.mortonCode >> lowBit) & bitMask;
      out[outIndex[bucket]++] = mp;
    }
  }

  if (nPasses & 1) // result is stored in tmp, swap
  {
    std::swap(*mortonPrimitives, tmp);
  }
}