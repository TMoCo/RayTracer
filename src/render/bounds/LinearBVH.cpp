#include <core/debug.h>
#include <render/bounds/LinearBVH.h>
#include <render/shapes/GLShapes.h>

#include <algorithm>

// TODO: use better allocation for data structure
LinearBVH::LinearBVH()
  : VAO{ 0 }, VBO{ 0 }, IBO{ 0 }, totalNodes{ 0 }, linearBVH{ nullptr }
{ }

LinearBVH::~LinearBVH()
{
  freeAligned(linearBVH);
}

LinearBVH::LinearBVH(std::vector<Shape*> inSceneShapes)
  : VAO{ 0 }, VBO{ 0 }, IBO{ 0 }, totalNodes{ 0 }, sceneShapes{ inSceneShapes }, linearBVH{ nullptr }
{
  if (sceneShapes.empty())
  {
    return;
  }

  std::vector<ShapePrimitiveInfo> shapeInfo(sceneShapes.size());
  for (int p = 0; p < shapeInfo.size(); ++p)
  {
    shapeInfo[p] = { p, sceneShapes[p]->getAABB() };
  }

  MemoryArena arena(1024 * 1024);

  std::vector<Shape*> orderedSceneShapes;
  orderedSceneShapes.reserve(sceneShapes.size());

  BVHNode* root;

  // root = buildTree(arena, shapeInfo, 0, (int)sceneShapes.size(), &totalNodes, orderedSceneShapes);
  root = buildHorizontalLinearBVH(arena, shapeInfo, &totalNodes, orderedSceneShapes);

  sceneShapes.swap(orderedSceneShapes); // swap with ordered scene shapes
  shapeInfo.resize(0);

  linearBVH = allocAligned<LinearBVHNode>(totalNodes); // allocate linearNodes 

  int offset = 0;
  flattenBVHTree(root, &offset);
}

void LinearBVH::draw(Shader* shader) const
{
  glBindVertexArray(VAO);
  glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, GLCube::indices, (GLsizei)totalNodes);
}

bool LinearBVH::intersect(const Ray& ray, Surfel* surfel)
{
  bool hit = false;
  uint32_t toVisitOffset = 0, currentNodeIndex = 0;
  uint32_t nodesToVisit[64];

  while (true)
  {
    const LinearBVHNode* node = &linearBVH[currentNodeIndex];
    if (node->bounds.intersect(ray))
    {
      if (node->nShapes > 0)
      {
        for (uint32_t i = 0; i < (uint32_t)node->nShapes; ++i)
        {
          sceneShapes[node->shapeOffset + i]->intersect(ray, surfel);
        }

        if (toVisitOffset == 0) // reached bottom of to visit stack
        {
          break;
        }

        currentNodeIndex = nodesToVisit[--toVisitOffset];
      }
      else
      {
        if (ray.negDir[node->axis])
        {
          nodesToVisit[toVisitOffset++] = currentNodeIndex + 1;
          currentNodeIndex = node->secondChildOffset;
        }
        else
        {
          nodesToVisit[toVisitOffset++] = node->secondChildOffset;
          currentNodeIndex = currentNodeIndex + 1;
        }
      }
    }
    else
    {
      if (toVisitOffset == 0)
      {
        break;
      }
      currentNodeIndex = nodesToVisit[--toVisitOffset];
    }
  }

  return ray.tMax < INFINITY;
}

LinearBVH::BVHNode* LinearBVH::buildTree(MemoryArena& arena, std::vector<ShapePrimitiveInfo>& shapeInfo, 
  int start, int end, int* totalNodes, std::vector<Shape*>& orderedShapes)
{
  BVHNode* node = arena.alloc<BVHNode>();
  (*totalNodes)++;

  AABB nodeBounds;
  for (int i = start; i < end; ++i)
  {
    nodeBounds.mergeWithAABB(shapeInfo[i].bounds);
  }

  int nShapes = end - start;
  if (nShapes == 1)
  {
    // leaf node
    int firstShapeOffset = (int)orderedShapes.size();
    for (int i = start; i < end; ++i)
    {
      int shapeNum = shapeInfo[i].num;
      orderedShapes.push_back(sceneShapes[shapeNum]);
    }

    node->makeLeafNode(firstShapeOffset, nShapes, nodeBounds);
  }
  else
  {
    // interior node
    AABB centroidBounds;
    for (int i = start; i < end; ++i)
    {
      centroidBounds.mergeWithPoint(shapeInfo[i].centroid);
    }

    int axis = centroidBounds.getMaximumExtent();
    int mid = (end - start) / 2;

    if (centroidBounds.bounds[0][axis] == centroidBounds.bounds[1][axis])
    {
      // leaf node if all centroid points are the same in split axis dimension
      int firstShapeOffset = (int)orderedShapes.size();
      for (int i = start; i < end; ++i)
      {
        int shapeNum = shapeInfo[i].num;
        orderedShapes.push_back(sceneShapes[shapeNum]);
      }
      node->makeLeafNode(firstShapeOffset, nShapes, nodeBounds);
    }
    else
    {
      // split axis in middle
      float axisMidPoint = (centroidBounds.bounds[0][axis] + centroidBounds.bounds[1][axis]) * 0.5f;
      ShapePrimitiveInfo* midShape = std::partition(&shapeInfo[start], &shapeInfo[end - 1] + 1, 
        [axis, axisMidPoint](ShapePrimitiveInfo& spi)
      {
        return spi.centroid[axis] < axisMidPoint;
      });
      mid = (int)(midShape - &shapeInfo[0]);
      // if (mid != start && mid != end)

      node->makeInteriorNode(axis,
        buildTree(arena, shapeInfo, start, mid, totalNodes, orderedShapes),
        buildTree(arena, shapeInfo, mid, end, totalNodes, orderedShapes));
    }
  }

  return node;
}

LinearBVH::BVHNode* LinearBVH::buildHorizontalLinearBVH(MemoryArena& arena, const std::vector<ShapePrimitiveInfo>& shapeInfo,
  int* totalNodes, std::vector<Shape*>& orderedShapes)
{

  AABB bounds; // bounds of all shape centroids
  for (uint32_t p = 0; p < shapeInfo.size(); ++p)
  {
    bounds.mergeWithPoint(shapeInfo[p].centroid);
  }

  std::vector<MortonPrimitive> mortonPrimitives(sceneShapes.size());
  int mortonBits = 10; // 10 bits per axis
  int mortonScale = 1 << mortonBits;

  for (uint32_t mp = 0; mp < mortonPrimitives.size(); ++mp)
  {
    mortonPrimitives[mp].shapeIndex = shapeInfo[mp].num;
    mortonPrimitives[mp].mortonCode = apply3DMortonEncoding(bounds.getOffset(shapeInfo[mp].centroid) * (float)mortonScale);
  }

  radixSortMortonPrimitives(&mortonPrimitives);

  // allocate BVHNodes 
  std::vector<LBVHTreelet> treeletsToBuild;
  for (int start = 0, end = 1; end <= (int)mortonPrimitives.size(); ++end)
  {
    uint32_t mask = 0b00111111111111000000000000000000;
    if (end == (int)mortonPrimitives.size() ||
      (mortonPrimitives[start].mortonCode & mask) != (mortonPrimitives[end].mortonCode & mask))
    {
      int nShapes = end - start;
      int maxNodes = 2 * nShapes;
      BVHNode* nodes = arena.alloc<BVHNode>((size_t)maxNodes, false);
      treeletsToBuild.push_back({ start, nShapes, nodes });

      start = end;
    }
  }

  int orderedShapesOffset = 0;
  orderedShapes.resize(sceneShapes.size());

  // build treelets into nodes
  for (uint32_t t = 0; t < treeletsToBuild.size(); ++t)
  {
    int nodesCreated = 0;
    int firstBitIndex = 29 - 12;
    LBVHTreelet& treelet = treeletsToBuild[t];
    treelet.nodes = buildTreelet(treelet.nodes, shapeInfo, &mortonPrimitives[treelet.startIndex], treelet.nShapes,
      &nodesCreated, orderedShapes, &orderedShapesOffset, firstBitIndex);
    *totalNodes += nodesCreated;
  }

  mortonPrimitives.resize(0);

  std::vector<BVHNode*> builtTreelets;
  builtTreelets.reserve(treeletsToBuild.size());

  for (LBVHTreelet& treelet : treeletsToBuild)
  {
    builtTreelets.push_back(treelet.nodes); // first node is treelet root
  }

  // using the treelet roots, build a BVH with SAH
  return buildUpperSAH(arena, builtTreelets, 0, (uint32_t)builtTreelets.size(), totalNodes);
}

uint32_t LinearBVH::apply3DMortonEncoding(const Vector3& point)
{
  return (shift10BitsLeftToEvery3rd((uint32_t)point[0])) | (shift10BitsLeftToEvery3rd((uint32_t)point[1]) << 1)
    | (shift10BitsLeftToEvery3rd((uint32_t)point[2]) << 2);
  // zyxzyxzyxzyxzyx ... 
}

// place the ith bit in the 3ith position (i < 10)
uint32_t LinearBVH::shift10BitsLeftToEvery3rd(uint32_t x)
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
  
  constexpr int bitsPerPass = 6;
  int nBits = 30;
  int nPasses = nBits / bitsPerPass;
  
  for (uint32_t pass = 0; pass < (uint32_t)nPasses; ++pass)
  {
    int lowBit = pass * bitsPerPass; // sort nBitsPerPass starting at lowBit

    std::vector<MortonPrimitive>& in = (pass & 1) ? tmp : *mortonPrimitives;
    std::vector<MortonPrimitive>& out = (pass & 1) ? *mortonPrimitives : tmp;

    constexpr int nBuckets = 1 << bitsPerPass;
    int bucketCount[nBuckets] = { 0 };

    constexpr int bitMask = nBuckets - 1;
    for (const MortonPrimitive& mp : in)
    {
      int bucket = (mp.mortonCode >> lowBit) & bitMask;
      ++bucketCount[bucket]; // count nb of values in each bucket
    }

    int outIndex[nBuckets]; // offsets into output array where bucket starts
    outIndex[0] = 0;
    for (uint32_t i = 1; i < nBuckets; ++i)
    {
      outIndex[i] = outIndex[i - 1] + bucketCount[i - 1];
    }

    // recompute bucket primitives land in and store in output array
    for (const MortonPrimitive& mp : in)
    {
      int bucket = (mp.mortonCode >> lowBit) & bitMask;
      out[outIndex[bucket]++] = mp;
    }
  }

  if (nPasses & 1) // result is stored in tmp, swap
  {
    std::swap(*mortonPrimitives, tmp);
  }
}

LinearBVH::BVHNode* LinearBVH::buildTreelet(BVHNode*& buildNodes, const std::vector<ShapePrimitiveInfo>& shapeInfo,
  MortonPrimitive* mortonPrimitives, int nShapes, int* totalNodes, std::vector<Shape*>& orderedShapes,
  int* orderedShapeOffset, int bitIndex)
{
  if (bitIndex == -1 || nShapes < MAX_SHAPES_PER_NODE)
  {
    (*totalNodes)++;
    BVHNode* node = buildNodes++; // advance current node
    AABB bounds;
    int firstShapeOffset = *orderedShapeOffset;
    *orderedShapeOffset += nShapes;

    for (int i = 0; i < nShapes; ++i)
    {
      uint32_t shapeIndex = mortonPrimitives[i].shapeIndex;
      orderedShapes[firstShapeOffset + i] = sceneShapes[shapeIndex];
      bounds.mergeWithAABB(shapeInfo[shapeIndex].bounds);
    }

    node->makeLeafNode(firstShapeOffset, nShapes, bounds);
    return node;
  }
  else
  {
    int mask = 1 << bitIndex;
    if ((mortonPrimitives[0].mortonCode & mask) == (mortonPrimitives[nShapes - 1].mortonCode & mask))
    {
      // no split for this bit index, advance to next index
      return buildTreelet(buildNodes, shapeInfo, mortonPrimitives, nShapes, totalNodes, orderedShapes, 
        orderedShapeOffset, bitIndex - 1);
    }
    
    // valid split, find split point with binary search
    int searchStart = 0, searchEnd = nShapes - 1;
    while (searchStart + 1 != searchEnd)
    {
      int mid = (searchStart + searchEnd) / 2; // >> 1

      if ((mortonPrimitives[searchStart].mortonCode & mask) == (mortonPrimitives[mid].mortonCode & mask))
      {
        searchStart = mid;
      }
      else
      {
        searchEnd = mid;
      }
    }

    int splitOffset = searchEnd; // build an interior node from this offset, recurse on left and right spans

    (*totalNodes)++;
    BVHNode* node = buildNodes++;

    BVHNode* childBVHNodes[2] =
    {
      // left
      buildTreelet(buildNodes, shapeInfo, mortonPrimitives, splitOffset, totalNodes, 
      orderedShapes, orderedShapeOffset, bitIndex - 1),
      // right
      buildTreelet(buildNodes, shapeInfo, &mortonPrimitives[splitOffset], nShapes - splitOffset, totalNodes,
      orderedShapes, orderedShapeOffset, bitIndex - 1)
    };

    int axis = bitIndex % 3;
    node->makeInteriorNode(axis, childBVHNodes[0], childBVHNodes[1]);
    return node;
  }
}

LinearBVH::BVHNode* LinearBVH::buildUpperSAH(MemoryArena& arena, std::vector<BVHNode*>& treeletRoots, int start, int end, int* totalNodes)
{
  int nNodes = end - start;
  if (nNodes == 1)
  {
    return treeletRoots[start];
  }

  (*totalNodes)++;
  BVHNode* node = arena.alloc<BVHNode>();

  AABB bounds; // bounds of all child nodes
  AABB centroidBounds; // LBVH bounds of node centroids
  for (int i = start; i < end; ++i)
  {
    bounds.mergeWithAABB(treeletRoots[i]->bounds);
    centroidBounds.mergeWithPoint(treeletRoots[i]->bounds.getCentroid());
  }

  int axis = centroidBounds.getMaximumExtent(); // partition split axis

  constexpr int nBuckets = 12;
  struct BucketInfo
  {
    int count = 0;
    AABB bounds;
  };

  BucketInfo buckets[nBuckets]; // buckets centroid of child nodes lie in

  for (int i = start; i < end; ++i)
  {
    // centroid along split axis
    float axisCentroid = (treeletRoots[i]->bounds.bounds[0][axis] + treeletRoots[i]->bounds.bounds[1][axis]) * 0.5f;
    int bucketIndex = (int)(nBuckets * ((axisCentroid - centroidBounds.bounds[0][axis]) /
      (centroidBounds.bounds[1][axis] - centroidBounds.bounds[0][axis])));
    
    if (bucketIndex == nBuckets)
    {
      bucketIndex = nBuckets - 1;
    }

    buckets[bucketIndex].count++;
    buckets[bucketIndex].bounds.mergeWithAABB(treeletRoots[i]->bounds);
  }

  // TODO: attempt linear complexity (first pass store count and boxes, second pass compute costs)
  // estimate traveral costs for each bucket based on surface area, number of nodes in bucket and a scalar for intersect overhead (eg is function virtual)
  float costs[nBuckets - 1];
  for (int i = 0; i < nBuckets - 1; ++i)
  {
    AABB b0, b1;
    int count0 = 0, count1 = 0;
    for (int j = 0; j <= i; ++j)
    {
      b0.mergeWithAABB(buckets[j].bounds);
      count0 += buckets[j].count;
    }
    for (int j = i + 1; j < nBuckets; ++j)
    {
      b1.mergeWithAABB(buckets[j].bounds);
      count1 += buckets[j].count;
    }
    // 
    costs[i] = 0.125f + ((count0 * b0.getSurfaceArea() + (count1 * b1.getSurfaceArea())) / bounds.getSurfaceArea());
  }

  float minCost = costs[0];
  int minCostBucketIndex = 0; // find bucket with minimum cost
  for (int i = 1; i < nBuckets - 1; ++i)
  {
    if (costs[i] < minCost)
    {
      minCost = costs[i];
      minCostBucketIndex = i;
    }
  }

  // split nodes based based on their buckets indices (left / right of minCost bucket)
  BVHNode** pMidNode = std::partition(&treeletRoots[start], &treeletRoots[end - 1] + 1,
    [=](const BVHNode* node) // lambda expression as predicate comparing bucket indices
  {
    float axisCentroid = (node->bounds.bounds[0][axis] + node->bounds.bounds[1][axis]) * 0.5f;
    int bucketIndex = (int) (nBuckets * ((axisCentroid - centroidBounds.bounds[0][axis]) /
      (centroidBounds.bounds[1][axis] - centroidBounds.bounds[0][axis])));
    
    if (bucketIndex == nBuckets)
    {
      bucketIndex = nBuckets - 1;
    }

    return bucketIndex <= minCostBucketIndex;
  });

  uint32_t mid = (uint32_t)(pMidNode - &treeletRoots[0]); // use address arithmetic to get mid index

  node->makeInteriorNode(axis, 
    buildUpperSAH(arena, treeletRoots, start, mid, totalNodes), 
    buildUpperSAH(arena, treeletRoots, mid, end, totalNodes));
  
  return node;
}

int LinearBVH::flattenBVHTree(BVHNode* node, int* offset)
{
  LinearBVHNode* linearNode = &linearBVH[*offset];
  
  linearNode->bounds = node->bounds;
  int newOffset = (*offset)++;
  
  if (node->nShapes > 0) // leaf
  {
    linearNode->shapeOffset = node->shapeOffset;
    linearNode->nShapes = node->nShapes;
  }
  else // interior
  {
    linearNode->axis = node->axisSplit;
    linearNode->nShapes = 0;
    flattenBVHTree(node->children[0], offset); // node's offset is first child offset
    linearNode->secondChildOffset = flattenBVHTree(node->children[1], offset);
  }
  return newOffset;
}

void LinearBVH::getGlData()
{
  // get AABBs
  std::vector<Matrix4> AABBTransforms;
  AABBTransforms.reserve(totalNodes);

  for (int i = 0; i < totalNodes; ++i)
  {
    AABB* bounds = &linearBVH[i].bounds;
    Vector3 position = bounds->getCentroid();
    Vector3 scale = bounds->getDiagonal() * 0.5f;
    AABBTransforms.push_back(Matrix4::translation(position) * Matrix4::scale(scale));
  }

  // instance buffer
  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ARRAY_BUFFER, IBO);
  glBufferData(GL_ARRAY_BUFFER, SIZE_OF_MAT4 * totalNodes, AABBTransforms.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glGetError();

  // vertex buffer
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLCube::unitCube), GLCube::unitCube, GL_STATIC_DRAW); // TODO: may cause errors

  // vertex attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SIZE_OF_VEC3, (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, IBO);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, SIZE_OF_MAT4, (void*)0);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, SIZE_OF_MAT4, (void*)SIZE_OF_VEC4);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, SIZE_OF_MAT4, (void*)(2 * SIZE_OF_VEC4));
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, SIZE_OF_MAT4, (void*)(3 * SIZE_OF_VEC4));

  glVertexAttribDivisor(1, 1);
  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);

  glBindVertexArray(0);
}