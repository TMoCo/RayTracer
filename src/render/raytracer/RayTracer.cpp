/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <xthreads.h>
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
  void rayTraceFast(const Scene* scene, RayTracerSettings settings, Image<byte_t>* raytraced)
  {
    auto rayTraceLoop = [settings, scene, raytraced](const uint32_t& begin, const uint32_t& end)
    {
      uint32_t width = (uint32_t)settings.imgDim[0], height = (uint32_t)settings.imgDim[1];
      float rWidth = 1.0f / (float)settings.imgDim[0], rHeight = 1.0f / (float)settings.imgDim[1];
      float inversNumSamples = 1.0f / (float)settings.nSamples;

      Colour colour;
      uint32_t vp_x = 0, vp_y = 0;
      size_t pixelOffset = 0;

      Image<float> local = { (int)(end - begin), 1, 3 };

      for (int sample = 0; sample < settings.nSamples; ++sample)
      {
        for (uint32_t i = begin; i < end; ++i)
        {
          vp_x = i % width;
          vp_y = i / height;
          colour = colour::Black;
            colour += rt::castRay(scene, Ray::getCameraRay(&scene->mainCamera,
              { (vp_x + random::ud_float_0_1(random::generator) * settings.aaKernel) * rWidth,
                (vp_y + random::ud_float_0_1(random::generator) * settings.aaKernel) * rHeight }), MAX_DEPTH);
            colour *= inversNumSamples;
          local.writePixelAt(pixelOffset++, &colour[0]);
        }
      }
      raytraced->writeBlockAt(local.size(), local[0], (size_t)begin * 3);
    };

    fprintf_s(stdout, "Ray Tracing...\t");
    auto t0 = sys_clock::now();
    parallel::pool.parallelFor(0u, (uint32_t)(settings.imgDim[0] * settings.imgDim[1]), rayTraceLoop);
    auto t1 = sys_clock::now();
    fprintf_s(stdout, " Finished! Took: %lli ms.\n", 
      std::chrono::duration_cast<std::chrono::milliseconds>(t1- t0).count());
  }

  void rayTrace(const Scene* scene, rt::RayTracerSettings settings, Image<byte_t>* raytraced, std::atomic<bool>& update,
    std::atomic<int>& taskCount, Profiler* profiler)
  {
#ifdef PARALLEL
    if (parallel::pool.numTasks() > 0)
    {
      profiler->addLogEntry("Tasks still running, please wait till image has rendered\n");
      return;
    }
#endif // !PARALLEL

    if (settings.imgDim[0] != raytraced->getWidth() ||
      settings.imgDim[1] != raytraced->getHeight())
    {
      raytraced->resize(settings.imgDim[0], settings.imgDim[1]);
    }

    raytraced->clear();

    auto rayTraceLoop = [settings, scene, raytraced, &update, &taskCount](const uint32_t& begin, const uint32_t& end)
    {
      uint32_t width = (uint32_t)settings.imgDim[0], height = (uint32_t)settings.imgDim[1];
      float rWidth = 1.0f / (float)settings.imgDim[0], rHeight = 1.0f / (float)settings.imgDim[1];
      float inversNumSamples = 1.0f / (float)settings.nSamples;

      Colour colour;
      uint32_t vp_x = 0, vp_y = 0;
      size_t pixelOffset = 0;

      Image<byte_t> local = { (int)(end - begin), 1, 3 };

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
      raytraced->writeBlockAt(local.size(), local[0], (size_t)begin * 3); // copy into whole image for now
      // update atomics
      update = true;
      taskCount++;
    };
  
#ifdef PARALLEL
    // divide the image into equal sized ranges
    uint32_t tasksTotal = (uint32_t)(settings.imgDim[0] * settings.imgDim[1]);
    uint32_t taskSize = tasksTotal / (uint32_t)parallel::pool.numThreads();
    if (profiler)
    {
      profiler->addLogEntry("Profiler settings: %i x %i image, %i samples.\n", 
        profiler->settings.imgDim[0], profiler->settings.imgDim[1], profiler->settings.nSamples);

      auto profilingLoop = [profiler, scene](const uint32_t& begin, const uint32_t& end)
      {
        uint32_t width = (uint32_t)profiler->settings.imgDim[0], height = (uint32_t)profiler->settings.imgDim[1];
        float rWidth = 1.0f / (float)width, rHeight = 1.0f / (float)height;

        uint32_t vp_x = 0, vp_y = 0;
        size_t pixelOffset = 0;

        Image<byte_t> local = { (int)(end - begin), 1, 3 };
        float aaKernel = profiler->settings.aaKernel;

        uint32_t threadId = _Thrd_id(); // c thread id

        Profiler::ThreadInfo& info = profiler->initThreadInfo(threadId, end - begin);
        
        uint32_t depth;
        auto start = sys_clock::now();
        for (uint32_t i = begin; i < end; ++i)
        {
          vp_x = i % width;
          vp_y = i / height;
          depth = 0;
          for (int sample = 0; sample < profiler->settings.nSamples; ++sample)
          {
            depth += rt::castProfilerRay(scene, Ray::getCameraRay(&scene->mainCamera,
              { (vp_x + random::ud_float_0_1(random::generator) * aaKernel) * rWidth,
                (vp_y + random::ud_float_0_1(random::generator) * aaKernel) * rHeight }), MAX_DEPTH);
          }
        }
        auto finish = sys_clock::now();
        info.runtime = (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
        profiler->addLogEntry("Thread %llu finished in %u ms.\n", threadId, info.runtime);
      };

      // launch threads and leave running
      for (uint32_t i = 0; i < parallel::pool.numThreads(); ++i)
      {
        parallel::pool.pushTask(profilingLoop, i * taskSize, 
          (i + 1ull) == parallel::pool.numThreads() ?  tasksTotal : (i + 1) * taskSize);
      }
    }
    else
    {
      for (uint32_t i = 0; i < parallel::pool.numThreads(); ++i)
      {
        parallel::pool.pushTask(rayTraceLoop, i * taskSize,
          (i + 1ull) == parallel::pool.numThreads() ? tasksTotal : (i + 1) * taskSize);
      }
    }
#else
    rayTraceLoop(0u, size);
    outImage->writeToImageFile(SCREENSHOTS + settings.imageName + ".jpg");
#endif // PARALLEL
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
    return colour::Black;
  }

  uint32_t castProfilerRay(const Scene* scene, const Ray& inRay, uint32_t depth)
  {
    if (depth == 0) // stop recursion 
    {
      return depth;
    }
    Surfel surfel;
    if (scene->intersect(inRay, &surfel))
    {
      Ray scattered;
      Colour attenuation;
      Colour emition = surfel.material->emit(surfel.uv);
      if (surfel.material->scatter(inRay, surfel, attenuation, scattered))
      {
        return rt::castProfilerRay(scene, scattered, --depth);
      }
    }
    return depth;
  }
}