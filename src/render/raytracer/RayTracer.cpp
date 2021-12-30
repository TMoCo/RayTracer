/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Raytracer class declaration
//

#include <core/core.h>
#include <core/random.h>
#include <render/materials/Material.h>
#include <render/raytracer/RayTracer.h>
#include <render/raytracer/UniformSampler.h>
#include <resource/ResourceManager.h>

#define MAX_DEPTH 10

RayTracer::RayTracer()
  : outputName{ "out" }, numSamples { 1 }, antiAliasingKernelSize{ 1.0f }, dimensions{ 500, 500 },
  rayTracedData{ 500, 500, 3 }
{ 
  ResourceManager::get().addTexture("ray traced output", new Texture{ &rayTracedData, GL_RGB });
}

void RayTracer::rayTrace(const Scene* scene, const Camera* camera, bool antiAliasing)
{
  // account for resizing
  if ((dimensions[0] != rayTracedData.getWidth()) || (dimensions[1] != rayTracedData.getHeight()))
  {
    rayTracedData.resize(dimensions[0], dimensions[1]);
  }

  // scale for aspect ratio
  UI32 width = rayTracedData.getWidth(), height = rayTracedData.getHeight();
  F32 rWidth = 1.0f / (F32)width, rHeight = 1.0f / (F32)height;
  F32 inversNumSamples  = 1.0f / (F32)numSamples;

  rayTracedData.clear(); // reset frame Image

  std::cout << "Ray tracing...";

  Colour colour;
  
  auto start = sys_clock::now();
  
  if (antiAliasing)
  {
    for (UI32 col = 0; col < height; ++col)
    { 
      for (UI32 row = 0; row < width; ++row)
      {
        colour = colour::Black;
        for (UI32 sample = 0; sample < (UI32)numSamples; ++sample)
        {
          colour += castRay(scene, Ray::generateCameraRay(camera, 
            { (row + random::uniformF32()) * rWidth, (col + random::uniformF32()) * rHeight }), MAX_DEPTH);
        }
        colour *= inversNumSamples;
        rayTracedData.writePixelColour(row, col, &colour[0]);
      }
    }
  }
  else
  {
    for (UI32 col = 0; col < height; ++col)
    {
      for (UI32 row = 0; row < width; ++row)
      {
        colour = colour::Black;
        for (UI32 sample = 0; sample < (UI32)numSamples; ++sample)
        {
          colour += castRay(scene, Ray::generateCameraRay(camera, { row * rWidth, col * rHeight }), MAX_DEPTH);
        }
        colour *= inversNumSamples;
        rayTracedData.writePixelColour(row, col, &colour[0]);
      }
    }
  }

  auto end = sys_clock::now();
  std::cout << "Finished!\nTook: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

  ResourceManager::get().getTexture("ray traced output")->generate(true); // load image texture for display

  // save to disk
  rayTracedData.writeToImageFile( SCREENSHOTS + outputName + ".jpg");
}

Colour RayTracer::castRay(const Scene* scene, const Ray& inRay, UI32 depth) const
{
  if (depth == 0) // stop recursion 
  {
    return colour::Black;
  }

  Surfel surfel;
  
  if (scene->intersect(inRay, &surfel))
  {
    Ray scattered;
    Colour attenuation;
    Colour emition = surfel.material->emit(surfel.uv);
      
    if (surfel.material->scatter(inRay, surfel, attenuation, scattered))
    {
      return emition + attenuation * castRay(scene, scattered, --depth);
    }

    return emition;
  }

  // TODO: Add background colour (from image, sample cube)
  return colour::Black;
  F32 t = 0.5f * (inRay.direction.normalize()[1] + 1.0f);
  return (1.0f - t) * colour::White + t * Colour{ 0.5f, 0.7f, 1.0f };
  /*
  */
}
