/*
* Author: Thomas Moreno Cooper
* Date created: 08/01/2022
* Copyright 2022 under the MIT license
*/

#ifndef PARALLEL_H
#define PARALLEL_H 1

#include <core/core.h>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <queue>
#include <functional>

namespace parallel
{
  // thread safe data structures
  template <typename T>
  class Queue
  {
  public:
    Queue();

    ~Queue();

    bool isEmpty();

    size_t size();

    void push(T& t);

    bool pop(T& t);

  private:
    std::queue<T> queue;

    std::mutex mutex;
  };

  template <typename T>
  Queue<T>::Queue()
  {}

  template <typename T>
  Queue<T>::~Queue()
  {}

  template <typename T>
  bool Queue<T>::isEmpty()
  {
    std::lock_guard<std::mutex> lock(mutex);
    return queue.empty();
  }

  template <typename T>
  size_t Queue<T>::size()
  {
    std::lock_guard<std::mutex> lock(mutex);
    return queue.size();
  }

  template <typename T>
  void Queue<T>::push(T& t)
  {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(std::move(t));
  }

  template <typename T>
  bool Queue<T>::pop(T& t)
  {
    std::lock_guard<std::mutex> lock(mutex);
    if (queue.empty())
    {
      return false;
    }
    t = std::move(queue.front());
    queue.pop(); // remove empty
    return true;
  }
  
  class ThreadPool
  {
  public:
    ThreadPool(const size_t& threadCount = std::thread::hardware_concurrency());

    ~ThreadPool();

    size_t numThreads() const;

    size_t numTasks() const;

    size_t numTasksRunning();

    template<typename F>
    void pushTask(const F& task);

    template<typename F, typename... A>
    void pushTask(const F& task, const A& ...args);

    template<typename T, typename F>
    void parallelFor(const T& start, const T& end, const F& loop, uint32_t blocks = 0);

    void waitForTasks();

  private:
    void worker();

    const size_t threadCount;

    size_t tasksCount;

    std::vector<std::thread> workers;

    std::atomic<bool> running;

    Queue<std::function<void()>> tasks;
  };

  ThreadPool::ThreadPool(const size_t& threadCount)
    : threadCount{ threadCount ? threadCount : std::thread::hardware_concurrency()},
    tasksCount{ 0 }, workers{}, running{ true }, tasks{}
  {
    workers.reserve(threadCount);
    for (size_t t = 0; t < threadCount; ++t)
    {
      workers.push_back(std::thread(&ThreadPool::worker, this));
    }
  }

  ThreadPool::~ThreadPool()
  {
    waitForTasks();
    running = false; 
    for (size_t t = 0; t < workers.size(); ++t)
    {
      workers[t].join();
    }
  }

  size_t ThreadPool::numThreads() const
  {
    return threadCount;
  }

  size_t ThreadPool::numTasks() const
  {
    return tasksCount;
  }

  size_t ThreadPool::numTasksRunning()
  {
    return tasksCount - tasks.size();
  }

  template<typename F>
  void ThreadPool::pushTask(const F& task)
  {
    tasksCount++;
    tasks.push(std::function<void()>(task));
  }

  template<typename F, typename... A>
  void ThreadPool::pushTask(const F& task, const A& ...args)
  {
    pushTask([task, args...]
      {
        task(args...);
      });
  }

  template<typename T, typename F>
  void ThreadPool::parallelFor(const T& start, const T& end, const F& loop, uint32_t blocks)
  {
    ERROR_MSG("Parallelise for loop into tasks not yet implemented"); // todo: parrallel for
  }

  void ThreadPool::waitForTasks()
  {
    while (true)
    {
      if (tasksCount == 0)
      {
        break;
      }
      std::this_thread::yield();
    }
  }

  void ThreadPool::worker()
  {
    while (running)
    {
      std::function<void()> task; // move task from queue to this handle
      if (tasks.pop(task))
      {
        task();
        tasksCount--;
      }
    }
  }
}

#endif // !PARALLEL_H

