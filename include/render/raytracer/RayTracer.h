/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Raytracer class 
//

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <core/core.h>

#include <resource/buffer.h>

#include <scene/Scene.h>

#include <render/Camera.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>
#include <render/primitives/Mesh.h>

#include <widgets/Window.h>

#define MAX_DEPTH 4

class RayTracer {
public:
  void raytrace(buffer<colour>& frameBuffer, Scene* scene, const Camera* camera, UI32 samples);
 
private:
  colour CastRay(const Ray& ray, UI32 depth);

  // TODO: move intersection test to respective shapes
  bool Intersect(const Ray& ray, const std::vector<Mesh*>& meshes, Surfel& surfel, F32& tNear);
    
  bool MollerTrumbore(const Ray& ray, const std::vector<Mesh*>& meshes, Surfel& surfel, F32& tNear);

  // compute lighting
  Vector3 RandomAreaLightPoint(const Mesh* light);
    
  // monte carlo sampling methods
  Vector2 UniformSampleDisk(const Vector2& uv);

  Vector3 UniformSampleHemisphere(const Vector2& uv);
    
  Vector3 CosineSampleHemisphere(const Vector2& uv);
  // slower but has better variance
  Vector3 UniformSampleTriangleBasuOwen(const F32& u);

  Vector3 UniformSampleTriangle(const Vector2& uv);
};

#endif 