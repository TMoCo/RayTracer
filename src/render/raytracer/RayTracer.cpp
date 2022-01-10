/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <core/core.h>
#include <core/debug.h>
#include <core/random.h>
#include <core/parallel.h> 
#include <render/materials/Material.h>
#include <render/raytracer/RayTracer.h>
#include <resource/ResourceManager.h>

#define PARALLEL 

#define MAX_DEPTH 10

namespace rt
{
  void rayTrace(const Scene* scene, rt::RayTracerSettings settings, Image* outImage)
  {
    if ((settings.imgDim[0] != outImage->getWidth()) || (settings.imgDim[1] != outImage->getHeight()))
    {
      outImage->resize(settings.imgDim[0], settings.imgDim[1]);
    }

    outImage->clear();

    // lambda expression for looping over scene and casting rays
    auto rayTraceLoop = [&settings, scene, outImage](const uint32_t& begin, const uint32_t& end)
    {
      uint32_t width = (uint32_t)settings.imgDim[0], height = (uint32_t)settings.imgDim[1];
      float rWidth = 1.0f / (float)settings.imgDim[0], rHeight = 1.0f / (float)settings.imgDim[1];
      float inversNumSamples = 1.0f / (float)settings.nSamples;

      Colour colour;
      uint32_t vp_x = 0, vp_y = 0;
      size_t pixelOffset = 0;

      // local image buffer TODO: test for different image heights
      // make the image large enough to hold the entire range 
      Image local = { (int)(end - begin), 1, 3 };
      DEBUG_PRINT("itertation range: %i, local image size: %llu\t", end - begin, local.size());

      if (settings.aaKernel)
      {
        for (uint32_t i = begin; i < end; ++i)
        {
          vp_x = i % width;
          vp_y = i / height;
          colour = colour::Black;
          for (int sample = 0; sample < settings.nSamples; ++sample)
          {
            colour += rt::castRay(scene, Ray::getCameraRay(&scene->mainCamera,
              { (vp_x + random::ud_float_0_1(random::generator) * settings.aaKernel) * rWidth,
                (vp_y + random::ud_float_0_1(random::generator) * settings.aaKernel) * rHeight }), MAX_DEPTH);
          }
          colour *= inversNumSamples;
          local.writePixelAt(pixelOffset++, &colour[0]);
        }
      }
      else
      {
        for (uint32_t i = begin; i < end; ++i)
        {
          vp_x = i % width;
          vp_y = i / height;
          colour = colour::Black;
          for (int sample = 0; sample < settings.nSamples; ++sample)
          {
            colour += 
              rt::castRay(scene, Ray::getCameraRay(&scene->mainCamera, { vp_x * rWidth, vp_y * rHeight }), MAX_DEPTH);
          }
          colour *= inversNumSamples;
          local.writePixelAt(pixelOffset++, &colour[0]);
        }
      }
      DEBUG_PRINT("Pixel offset is %llu, local should accomodate %llu bytes", pixelOffset, pixelOffset * 3);
      DEBUG_PRINT("\nDim: %i, %i\n", local.getWidth(), local.getHeight());
      DEBUG_PRINT("\nWriting to out image: Bytes to write: %llu\tByte offset: %u\n", local.size(), begin * 3);
      outImage->writeBlockAt(local.size(), local[0], (size_t)begin * 3); // copy into whole image for now
      DEBUG_PRINT("Finished writing.\n");
    };
  
    uint32_t size = (uint32_t)(settings.imgDim[0] * settings.imgDim[1]);
    
    DEBUG_PRINT("Raytracing...\t");
    
    auto start = sys_clock::now();
#ifdef PARALLEL
    // use all cores up
    parallel::pool.parallelFor(0u, size, rayTraceLoop);
    /*
    // todo split image better
    parallel::pool.pushTask(rayTraceLoop, 0, size / 2u);
    parallel::pool.pushTask(rayTraceLoop, size / 2u, size);
    parallel::pool.waitForTasks(); // update on each task end when waiting here...
    */

    outImage->writeToImageFile(SCREENSHOTS + settings.imageName + ".jpg");
#else
    rayTraceLoop(0u, size);
    outImage->writeToImageFile(SCREENSHOTS + settings.imageName + ".jpg");
#endif // PARALLEL

    auto finish = sys_clock::now();
    DEBUG_PRINT("Finished!\nTook: % llu ms\n",
      std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count());
    fflush(stderr);
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