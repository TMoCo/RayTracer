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
#define SUBTASKS 20
#define MAX_DEPTH 10

namespace rt
{
  void rayTraceFast(const Scene* scene, RayTracerSettings settings, Image<byte_t>* raytraced)
  {
    auto rayTraceLoop = [settings, scene, raytraced](const uint32_t& begin, const uint32_t& end)
    {
      uint32_t width = (uint32_t)settings.imgDim[0], height = (uint32_t)settings.imgDim[1];
      float rWidth = 1.0f / (float)width, rHeight = 1.0f / (float)height, inversNumSamples = 1.0f / (float)settings.nSamples;

      Image<byte_t> local = { (int)(end - begin), 1, 3 }; // thread local image section

      uint32_t vp_x = 0, vp_y = 0;
      
      byte_t* pixel; // pixel indexing
      size_t pixelOffset = 0;

      Colour c;
      for (uint32_t i = begin; i < end; ++i)
      {
        vp_x = i % width;
        vp_y = i / height;

        c = colour::Black;
        
        for (int sample = 0; sample < settings.nSamples; ++sample)
        {
          c += rt::castRay(scene, Ray::getCameraRay(&scene->mainCamera,
            { (vp_x + random::ud_float_0_1(random::generator) * settings.aaKernel) * rWidth,
              (vp_y + random::ud_float_0_1(random::generator) * settings.aaKernel) * rHeight }), MAX_DEPTH);
        }
        
        c *= inversNumSamples;

        pixel = local[0] + pixelOffset;
        
        *pixel       = (byte_t)(sqrtf(c[0] / (c[0] + 1.0f)) * 255.0f);
        *(pixel + 1) = (byte_t)(sqrtf(c[1] / (c[1] + 1.0f)) * 255.0f);
        *(pixel + 2) = (byte_t)(sqrtf(c[2] / (c[2] + 1.0f)) * 255.0f);

        pixelOffset += 3;
      }
      raytraced->writeBlockAt(local.size(), local[0], (size_t)begin * 3);
    };

    fprintf_s(stdout, "Ray Tracing...\t");

    // launch thread jobs
    uint32_t imageSize = (uint32_t)(settings.imgDim[0] * settings.imgDim[1]);

    auto t0 = sys_clock::now();
#ifdef SUBTASKS
    uint32_t taskSize = imageSize / SUBTASKS;
    for (int i = 0; i < SUBTASKS; ++i)
    {
      parallel::pool.pushTask(rayTraceLoop, i * taskSize, (i + 1) == SUBTASKS ? imageSize : (i + 1) * taskSize);
    }
    parallel::pool.waitForTasks();
#else
    parallel::pool.parallelFor(0u, imageSize, rayTraceLoop);
#endif // SUBTASKS
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
      fprintf(stdout, "Cannot add tasks, already rendering\n");
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

      Colour c;
      uint32_t vp_x = 0, vp_y = 0;

      Image<float> localHDR  = { (int)(end - begin), 1, 3 };
      Image<byte_t> localLDR = { (int)(end - begin), 1, 3 };
      localHDR.clear(); // to zero
      localLDR.clear();

      size_t pixelOffset;
      float* c0;
      byte_t* c1;
      for (int sample = 0; sample < settings.nSamples; ++sample)
      {
        pixelOffset = 0;
        for (uint32_t i = begin; i < end; ++i)
        {
          vp_x = i % width;
          vp_y = i / height;

          c = rt::castRay(scene, Ray::getCameraRay(&scene->mainCamera,
            { (vp_x + random::ud_float_0_1(random::generator) * settings.aaKernel) * rWidth,
              (vp_y + random::ud_float_0_1(random::generator) * settings.aaKernel) * rHeight }),
            MAX_DEPTH);

          c0 = localHDR[0] + pixelOffset;
          *c0       += c[0]; // r
          *(c0 + 1) += c[1]; // g
          *(c0 + 2) += c[2]; // b
          
          // r
          c1 = localLDR[0] + pixelOffset;
          *c1 = (byte_t)(sqrtf( ((*c0) * inversNumSamples) / (((*c0) * inversNumSamples) + 1.0f)) * 255.0f);

          // g
          c0 += 1;
          c1 += 1;
          *c1 = (byte_t)(sqrtf( ((*c0) * inversNumSamples) / (((*c0) * inversNumSamples) + 1.0f)) * 255.0f);

          // b
          c0 += 1;
          c1 += 1;
          *c1 = (byte_t)(sqrtf( ((*c0) * inversNumSamples) / (((*c0) * inversNumSamples) + 1.0f)) * 255.0f);
          
          pixelOffset += 3;
        }

        if (raytraced->tryWriteBlockAt(localLDR.size(), localLDR[0], (size_t)begin * 3))
        {
          update = true;
        }
        taskCount++;
      }

      raytraced->writeBlockAt(localLDR.size(), localLDR[0], (size_t)begin * 3);
      update = true;
    };
  
#ifdef PARALLEL
    // divide the image into equal sized ranges
    uint32_t tasksTotal = (uint32_t)(settings.imgDim[0] * settings.imgDim[1]);
    uint32_t taskSize = tasksTotal / (uint32_t)parallel::pool.numThreads();
    if (profiler)
    {
      profiler->heatMap.resize(settings.imgDim[0], settings.imgDim[1]);
      profiler->heatMap.clear();
      profiler->addLogEntry("Profiler settings: %i x %i image, %i samples.\n", 
        profiler->settings.imgDim[0], profiler->settings.imgDim[1], profiler->settings.nSamples);

      auto profilingLoop = [profiler, scene](const uint32_t& begin, const uint32_t& end)
      {
        uint32_t width = (uint32_t)profiler->settings.imgDim[0], height = (uint32_t)profiler->settings.imgDim[1];
        float rWidth = 1.0f / (float)width, rHeight = 1.0f / (float)height;
        float rsamples = 1.0f / (float) profiler->settings.nSamples;
        float aaKernel = profiler->settings.aaKernel;

        uint32_t vp_x = 0, vp_y = 0;
        size_t pixelOffset = 0;

        Image<byte_t> local = { (int)(end - begin), 1, 3 };

        uint32_t threadId = _Thrd_id(); // c thread id

        Profiler::ThreadInfo& info = profiler->initThreadInfo(threadId, end - begin);
        
        auto start = sys_clock::now();
        for (uint32_t i = begin; i < end; ++i)
        {
          vp_x = i % width;
          vp_y = i / height;
          float depth = 0;
          for (int sample = 0; sample < profiler->settings.nSamples; ++sample)
          {
            depth += rt::castProfilerRay(scene, Ray::getCameraRay(&scene->mainCamera,
              { (vp_x + random::ud_float_0_1(random::generator) * aaKernel) * rWidth,
                (vp_y + random::ud_float_0_1(random::generator) * aaKernel) * rHeight }), 0);
          }
          byte_t* p = local[0] + pixelOffset;
          float value = depth * rsamples * 0.2f; 
          byte_t r = (byte_t)std::max(0.0f, 255.0f * (value - 1.0f));
          byte_t b = (byte_t)std::max(0.0f, 255.0f * (1.0f - value));
          *p       = r;
          *(p + 2) = b;
          *(p + 1) = 255 - r - b;

          pixelOffset += 3;
        }
        auto finish = sys_clock::now();

        info.runtime = (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

        profiler->addLogEntry("Thread %llu finished in %u ms.\n", threadId, info.runtime);

        while (!profiler->updateProfilerThreadData(threadId)) // collect data
        {
          std::this_thread::yield();
        }

        profiler->heatMap.writeBlockAt(local.size(), local[0], (size_t)begin * 3);
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

  float castProfilerRay(const Scene* scene, const rt::Ray& inRay, float depth)
  {
    if (depth == MAX_DEPTH) // stop recursion 
    {
      return depth;
    }
    Surfel surfel;
    if (scene->intersect(inRay, &surfel))
    {
      rt::Ray scattered;
      Colour attenuation;
      Colour emition = surfel.material->emit(surfel.uv);
      if (surfel.material->scatter(inRay, surfel, attenuation, scattered))
      {
        return rt::castProfilerRay(scene, scattered, ++depth);
      }
    }
    return depth;
  }
}