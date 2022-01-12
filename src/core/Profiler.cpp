/*
* Author: Thomas Moreno Cooper
* Date created: 10/01/2022
* Copyright 2022 under the MIT license
*/

#include <core/debug.h>
#include <core/Profiler.h>
#include <implot.h>

Profiler::Profiler()
  : settings{ "profiler_output", { 500, 500 }, 5, 1.0f }, threads{}, buffer{}, offsets{ 0 }, heatMap{ 0, 0, 3 }
{}

void Profiler::drawGui()
{
  int size = (int)offsets.size();

  ImGuiListClipper clipper; // clip the buffer string
  clipper.Begin(size);

  const char* begin = buffer.begin();
  const char* end = buffer.end();

  while (clipper.Step())
  {
    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
    {
      const char* line_start = begin + offsets[line_no];
      const char* line_end = (line_no + 1 < size) ? (begin + offsets[line_no + 1] - 1) : end;
      ImGui::TextUnformatted(line_start, line_end);
    }
  }
  clipper.End();
}

void Profiler::addLogEntry(const char* format, ...) // variadic 
{
  int oldSize = buffer.size();
  va_list args;
  __crt_va_start(args, format);
  buffer.appendfv(format, args);
  __crt_va_end(args);
  int newSize = buffer.size();
  while (oldSize < newSize)
  {
    if (buffer[oldSize] == '\n')
    {
      offsets.push_back(oldSize + 1);
    }
    oldSize++;
  }
}

Profiler::ThreadInfo& Profiler::initThreadInfo(uint32_t threadIndex, uint32_t taskSize)
{
  std::lock_guard<std::mutex> lock(mutex); // needed?
  Profiler::ThreadInfo& info = threads[threadIndex];
  info.id = threadIndex;
  info.pixels = taskSize;
  addLogEntry("Thread %u launched with %u pixels\n", threadIndex, taskSize);
  return info;
}

bool Profiler::updateProfilerThreadData(uint32_t threadId)
{
  bool ret = false;
  std::lock_guard<std::mutex> lock(mutex);

  ThreadInfo& info = threads[threadId];
  
  size_t taskSize = info.pixels;    
  
  if (taskSize > plotAxis.size())
  {
    size_t start = plotAxis.size();
    plotAxis.resize(plotAxis.size() + taskSize - plotAxis.size()); // enough to accomodate new x
    for (start; start < plotAxis.size(); ++start)
    {
      plotAxis[start] = (uint32_t)start;
    }
  }
  ret = true;
  return ret;
}

void Profiler::clearLog()
{
  buffer.clear();
  offsets = { 0 };
}

void Profiler::writeLogToFile()
{
  FILE* logOut;
  if (fopen_s(&logOut, "profiler_log.txt", "w") != 0)
  {
    ERROR_MSG("Failed to open log file.");
  }

  if (logOut)
  {
    fprintf_s(logOut, buffer.c_str());
    fclose(logOut);
  }
}