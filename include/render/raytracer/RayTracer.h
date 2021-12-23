/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Raytracer class 
//

#ifndef RAYTRACER_H
#define RAYTRACER_H 1

#include <image/Colour.h>
#include <image/Image.h>
#include <render/Camera.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>
#include <render/primitives/Mesh.h>
#include <scene/Scene.h>
#include <widgets/Window.h>

class RayTracer 
{
public:
  void raytrace(const Scene* scene, Image& image, const Camera* camera, UI32 samples) const;
 
private:
  Colour castRay(const Scene* scene, const Ray& ray, UI32 depth) const;

};

#endif 