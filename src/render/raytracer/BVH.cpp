#include <render/raytracer/BVH.h>

void BVH::getPrimitives(const std::vector<Mesh*>& meshes)
{
  // hard assumption: only triangular meshes
  UI64 numPrimitives = 0;
  for (auto& mesh : meshes)
    numPrimitives += mesh->indices.size() / 3;

  primitives.reserve(numPrimitives);

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
      primitives.push_back(prim);
    }
  }
}

void BVH::generateBuffers()
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
    memcpy(&data[i * AABBsize], primitives[i].bbox.getGlBufferData().data(), AABBsize * sizeof(F32));
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