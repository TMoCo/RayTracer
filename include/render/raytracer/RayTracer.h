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

#include <image/Colour.h>
#include <resource/buffer.h>
#include <render/Camera.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>
#include <render/primitives/Mesh.h>
#include <scene/Scene.h>
#include <widgets/Window.h>

#define MAX_DEPTH 4

class RayTracer {
public:
  void raytrace(buffer<colour>& frameBuffer, const Camera* camera, UI32 samples) const;

  void setScene(Scene* scene);
 
private:
  colour castRay(const Ray& ray, UI32 depth) const;

  bool mollerTrumbore(const Ray& ray, const std::vector<Mesh*>& meshes, Surfel& surfel, F32& tNear) const;

  Vector3 randomAreaLightPoint(const Mesh* light) const;
    
  Vector2 uniformSampleDisk(const Vector2& uv) const;

  Vector3 uniformSampleHemisphere(const Vector2& uv) const;
  
  Vector3 uniformSampleHemisphere(const Vector3& normal) const;

  Vector3 uniformSampleHemisphere(const Vector2& uv, const Vector3& normal) const;
  
  Vector3 uniformSampleUnitSphere() const;

  Vector3 cosineSampleHemisphere(const Vector2& uv) const;

  Vector3 uniformSampleTriangleBasuOwen(const F32& u) const; // slower but has better variance

  Vector3 uniformSampleTriangle(const Vector2& uv) const;

  Scene* scene;

};

#endif 