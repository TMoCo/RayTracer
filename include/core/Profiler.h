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
#include <unordered_map>
#include <vector>

// measure thread usage 
class Profiler
{
public:
  Profiler();

  void drawGui();

  void addLogEntry(const char* format, ...);

  rt::RayTracerSettings settings;

  struct ThreadInfo
  {
    size_t id;
    uint32_t pixels;
    uint32_t runtime;
  };

  std::unordered_map<std::thread::id, ThreadInfo> threads;

private:
  void clearLog();

  void writeLogToFile();

  ImGuiTextBuffer buffer;

  std::vector<uint32_t> offsets;
};

#endif // !PROFILER_H 
