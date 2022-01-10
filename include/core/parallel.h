/*
* Author: Thomas Moreno Cooper
* Date created: 08/01/2022
* Copyright 2022 under the MIT license
*/

#ifndef PARALLEL_H
#define PARALLEL_H 1

#include <core/core.h>
#include <core/Profiler.h>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <queue>
#include <functional>

namespace parallel
{
  static constexpr size_t threadSleepDuration = 500;

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
    inline ThreadPool(const size_t& threadCount = std::thread::hardware_concurrency());

    inline ~ThreadPool();

    inline size_t numThreads() const;

    inline size_t numTasks() const;

    inline size_t numTasksRunning();

    template<typename F>
    inline void pushTask(const F& task);

    template<typename F, typename... A>
    inline void pushTask(const F& task, const A& ...args);

    template<typename T, typename F>
    inline void parallelFor(const T& start, const T& finish, const F& loop, size_t numBlocks = 0);

    inline void waitForTasks();

  private:
    inline void worker();

    inline void threadSleep();

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
  void ThreadPool::parallelFor(const T& start, const T& finish, const F& forLoop, size_t numBlocks)
  {
    if (start == finish)
    {
      return;
    }

    if (numBlocks == 0)
    {
      numBlocks = threadCount;
    }

    size_t total = (size_t)(finish - start);
    size_t blockSize = (size_t)(total / numBlocks);

    if (blockSize == 0) // more blocks than elements
    {
      blockSize = 1;
      numBlocks = total > 1 ? total : 1;
    }

    std::atomic<int> blocksRunning = 0;
    for (size_t i = 0; i < numBlocks; ++i)
    {
      T begin = (T)(i * blockSize) + start;
      T end = i == (numBlocks - 1) ? finish : (T)((i + 1) * blockSize) + start;
      blocksRunning++;
      pushTask([begin, end, &forLoop, &blocksRunning]
      {
        forLoop(begin, end);
        blocksRunning--;
      });
    }

    while (blocksRunning.load() != 0)
    {
      threadSleep();
    }
  }

  void ThreadPool::waitForTasks()
  {
    while (true)
    {
      if (tasksCount == 0)
      {
        break;
      }
      threadSleep();
      // std::this_thread::yield();
    }
  }

  void ThreadPool::worker()
  {
    std::function<void()> task; // move task from queue to this handle
    while (running)
    {
      if (tasks.pop(task))
      {
        std::cout << "in loop thread " << std::this_thread::get_id() << "\n";

        task();
        tasksCount--;
      }
    }
  }
  
  void ThreadPool::threadSleep()
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(threadSleepDuration));
  }
  
  static ThreadPool pool{ };
}

#endif // !PARALLEL_H

