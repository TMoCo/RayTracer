/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Raytracer class declaration
//

#include <core/core.h>
#include <core/debug.h>
#include <core/random.h>
#include <render/materials/Material.h>
#include <render/raytracer/RayTracer.h>
#include <render/raytracer/sample.h>
#include <resource/ResourceManager.h>
#include <core/parallel.h> 

#define PARALLEL 

#define MAX_DEPTH 10

namespace rt
{
  void rayTrace(const Scene* scene, rt::RayTracerSettings settings, Image& outImage)
  {
    // account for resizing 
    if ((settings.imgDim[0] != outImage.getWidth()) || (settings.imgDim[1] != outImage.getHeight()))
    {
      outImage.resize(settings.imgDim[0], settings.imgDim[1]);
    }

    outImage.clear();

    // lambda expression for looping over scene and casting rays
    auto rayTraceLoop = [&settings, scene, &outImage](const uint32_t& begin, const uint32_t& end)
    {
      uint32_t width = (uint32_t)settings.imgDim[0], height = (uint32_t)settings.imgDim[1];
      float rWidth = 1.0f / (float)settings.imgDim[0], rHeight = 1.0f / (float)settings.imgDim[1];
      float inversNumSamples = 1.0f / (float)settings.nSamples;

      Colour colour;
      uint32_t vp_x = 0, vp_y = 0;
      size_t pixelOffset = 0;

      // local image buffer TODO: test for different image heights
      Image local = { settings.imgDim[0] / ((settings.imgDim[0] * settings.imgDim[1]) / (int)(end - begin)), settings.imgDim[1], outImage.getChannels() };

      for (uint32_t i = begin; i < end; ++i)
      {
        vp_x = i % width;
        vp_y = i / height;
        colour = colour::Black;
        for (int sample = 0; sample < settings.nSamples; ++sample)
        {
          colour += rt::castRay(scene, Ray::generateCameraRay(&scene->mainCamera,
            { (vp_x + random::ud_float_0_1(random::generator)) * rWidth,
              (vp_y + random::ud_float_0_1(random::generator)) * rHeight }), MAX_DEPTH);
        }
        colour *= inversNumSamples;
        local.writePixelAt(pixelOffset++, &colour[0]);
      }

      outImage.writeBlockAt(local.size(), local[0], (size_t)begin); // copy into whole image for now
    };
  
    if (settings.aaOn)
    {
      // move to a single for loop
      parallel::pool.parallelFor(0u, (uint32_t)(settings.imgDim[0] * settings.imgDim[1]), rayTraceLoop);
    }
    else
    {
      auto start = sys_clock::now();
      uint32_t size = (uint32_t)(settings.imgDim[0] * settings.imgDim[1]);
#ifdef PARALLEL
      // use all cores up
      parallel::pool.parallelFor(0u, (uint32_t)(settings.imgDim[0] * settings.imgDim[1]), rayTraceLoop);

      // split into 4 cores
      /*
      parallel::pool.pushTask(rayTraceLoop, 0u, size / 4u);
      parallel::pool.pushTask(rayTraceLoop, size / 4u, size / 2u);
      parallel::pool.pushTask(rayTraceLoop, size / 2u, 3 * size / 4u);
      parallel::pool.pushTask(rayTraceLoop, 3 * size / 4u, size);
      parallel::pool.waitForTasks(); // update on each task end when waiting here...
      */
      
      outImage.writeToImageFile(SCREENSHOTS + settings.imageName + ".jpg");
#else
      rayTraceLoop(0u, size);
      outImage.writeToImageFile(SCREENSHOTS + settings.imageName + ".jpg");
#endif // PARALLEL
      auto finish = sys_clock::now();
      fprintf_s(stdout, "Finished!\nTook: %llu ms\n", 
        std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count());
    }
  }

  Colour castRay(const Scene* scene, const Ray& inRay, uint32_t depth)
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
        return emition + attenuation * rt::castRay(scene, scattered, --depth);
      }

      return emition;
    }

    // TODO: Add background colour (from image, sample cube)
    /*
    float t = 0.5f * (inRay.direction.normalize()[1] + 1.0f);
    return (1.0f - t) * colour::White + t * Colour{ 0.5f, 0.7f, 1.0f };
    */
    return colour::Black;
  }
}


RayTracer::RayTracer()
  : outputName{ "out" }, numSamples{ 1 }, antiAliasingKernelSize{ 1.0f }, dimensions{ 500, 500 },
  rayTracedData{ 500, 500, 3 }, antiAliasing{ false }
{ 
  ResourceManager::get().addTexture("ray traced output", new Texture{ &rayTracedData, GL_RGB });
}

void RayTracer::rayTrace(const Scene* scene, bool toGlTexture)
{
  /*
  // account for resizing 
  if ((dimensions[0] != rayTracedData.getWidth()) || (dimensions[1] != rayTracedData.getHeight()))
  {
    rayTracedData.resize(dimensions[0], dimensions[1]);
  }

  // scale for aspect ratio
  int width = rayTracedData.getWidth(), height = rayTracedData.getHeight();
  float rWidth = 1.0f / (float)width, rHeight = 1.0f / (float)height;
  float inversNumSamples  = 1.0f / (float)numSamples;

  rayTracedData.clear(); // reset frame Image

  std::cout << "Ray tracing...";

  Colour colour;
  
  auto start = sys_clock::now();
  
  if (antiAliasing)
  {
    // move to a single for loop
    float samples = numSamples;
    const Scene* s = scene;
    parallel::pool.parallelFor(0u, (uint32_t)(width * height), [&width, &height, &samples, &s](const uint32_t& begin, const uint32_t& end)
    {
      float rWidth = 1.0f / (float)width, rHeight = 1.0f / (float)height;
      float inversNumSamples = 1.0f / (float)samples;
      uint32_t row = 0, col = 0;
      Colour colour;
      // local copy of image
      for (uint32_t i = 0; i < (uint32_t)(width * height); ++i)
      {
        col = i / height;
        row = i % width;
        colour = colour::Black;
        for (int sample = 0; sample < (int)samples; ++sample)
        {
          colour += castRay(s, Ray::generateCameraRay(&s->mainCamera,
            { (row + random::ud_float_0_1(random::generator)) * rWidth, (col + random::ud_float_0_1(random::generator)) * rHeight }), MAX_DEPTH);
        }
        colour *= inversNumSamples;
        // rayTracedData.writePixelColour(row, col, &colour[0]);
      }
    });
  }
  else
  {
    for (uint32_t col = 0; col < height; ++col)
    {
      for (uint32_t row = 0; row < width; ++row)
      {
        colour = colour::Black;
        for (uint32_t sample = 0; sample < (uint32_t)numSamples; ++sample)
        {
          colour += castRay(scene, Ray::generateCameraRay(&scene->mainCamera, { row * rWidth, col * rHeight }), MAX_DEPTH);
        }
        colour *= inversNumSamples;
        rayTracedData.writePixelColour(row, col, &colour[0]);
      }
    }
  }

  auto end = sys_clock::now();
  std::cout << "Finished!\nTook: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
  if (toGlTexture)
  {
    ResourceManager::get().getTexture("ray traced output")->generate(true); // load image texture for display
  }
  rayTracedData.writeToImageFile( SCREENSHOTS + outputName + ".jpg");
  */
}

Colour castRay(const Scene* scene, const Ray& inRay, uint32_t depth)
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
  /*
  float t = 0.5f * (inRay.direction.normalize()[1] + 1.0f);
  return (1.0f - t) * colour::White + t * Colour{ 0.5f, 0.7f, 1.0f };
  */
  return colour::Black;
}
