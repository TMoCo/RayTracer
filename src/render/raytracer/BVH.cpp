#include <render/raytracer/BVH.h>

void BVH::buildBVH(const std::vector<Mesh*>& meshes)
{
  // from meshes
  getPrimitives(meshes);

  std::vector<Primitive*> ordered;

  // build nodes
  BVHNode* root = buildNode(0, primitives.size(), ordered);

}

BVHNode* BVH::buildNode(UI32 start, UI32 end, std::vector<Primitive*>& ordered)
{
  BVHNode* node = new BVHNode;
  numNodes++;
  AABB bbox{};
  for (UI32 i = start; i < end; ++i)
    bbox = AABB::mergeAABB(bbox, data[i].bbox);

  UI32 nPrim = end - start;
  if (nPrim == 1)
  {
    // leaf node
    UI32 firstPrim = ordered.size();
    for (UI32 i = start; i < end; ++i)
      ordered.push_back(&data[i]);
    node->initLeaf(firstPrim, nPrim, bbox);
  }
  else
  {
    AABB centroidBound{}; // get bounding box of primitive centroids
    for (UI32 i = start; i < end; ++i)
      centroidBound = AABB::mergeAABB(centroidBound, data[i].bbox.centre);
    // get axis with max extent
    AXIS axis = centroidBound.maxExtent();
    UI32 mid = (start + end) >> 1;
    if (centroidBound.max[axis] == centroidBound.min[axis])
    {
      // leaf node
      UI32 firstPrim = ordered.size();
      for (UI32 i = start; i < end; ++i)
        ordered.push_back(&data[i]);
      node->initLeaf(firstPrim, nPrim, bbox);
      return node;
    }
    else
    {
      // internal node
      node->initInternal(axis, 
        buildNode(start, mid, ordered),
        buildNode(mid, end, ordered));
    }
  }
  return node;
}

// hard assumption: only triangular meshes
void BVH::getPrimitives(const std::vector<Mesh*>& meshes)
{
  // clear previous data
  primitives.clear();
  primitives.reserve(numPrimitives);

  data.clear();
  data.reserve(numPrimitives);

  numPrimitives = 0;

  for (auto& mesh : meshes)
  {
    for (UI64 i = 0; i < mesh->indices.size() / 3; ++i)
    {
      // for each triangle, generate a primitive object
      Primitive prim{};
      prim.index = static_cast<UI32>(primitives.size());
      prim.bbox = AABB::getAABB({ 
        mesh->positions[mesh->indices[3 * i]],
        mesh->positions[mesh->indices[3 * i + 1]],
        mesh->positions[mesh->indices[3 * i + 2]] });
      prim.mesh = mesh;
      // push primitive
      data.push_back(prim);
      primitives.push_back(&data.back());
    }
  }
  numPrimitives = primitives.size();
}

void BVH::generatebuffers()
{
  // loop 
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  UI64 AABBsize = 24; // = num floats to draw AABB
  std::vector<F32> data{};
  std::vector<UI32> indices{};

  data.resize(primitives.size() * AABBsize);
  indices.reserve(primitives.size() * 36);

  // loop over each primitive and copy into array
  for (UI64 i = 0; i < primitives.size(); ++i)
  {
    // copy positions
    // memcpy(&data[i * AABBsize], primitives[i].bbox.getGlbufferData().data(), AABBsize * sizeof(F32));
    // copy indices with offset
    for (UI32 j = 0; j < 36; ++j)
      indices.push_back(AABB::indices[j] + i * 8);
  }
  
  glBindVertexArray(vao);

  // generate index buffer data
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(F32), data.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(UI32), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}

void BVH::draw()
{
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, primitives.size() * 36, GL_UNSIGNED_INT, 0);
}