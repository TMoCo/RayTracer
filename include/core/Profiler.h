/*
* Author: Thomas Moreno Cooper
* Date created: 10/01/2022
* Copyright 2022 under the MIT license
*/

#ifndef PROFILER_H
#define PROFILER_H 1 

#include <error.h>
#include <imgui.h>
#include <render/raytracer/RayTracer.h>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <vector>

// measure thread usage 
class Profiler
{
public:
  struct ThreadInfo
  {
    uint32_t id;
    uint32_t pixels;
    uint32_t runtime;
  };

  Profiler();

  void drawGui(bool viewPlot = false);

  void addLogEntry(const char* format, ...);

  ThreadInfo& initThreadInfo(uint32_t threadIndex, uint32_t taskSize);

  rt::RayTracerSettings settings;

  std::unordered_map<uint32_t, ThreadInfo> threads;

private:
  void clearLog();

  void writeLogToFile();

  ImGuiTextBuffer buffer;

  std::vector<uint32_t> offsets;

  std::mutex mutex;
};

#endif // !PROFILER_H 
