/*
* Author: Thomas Moreno Cooper
* Date created: 08/01/2022
* Copyright 2022 under the MIT license
*/

#ifndef TEST_H
#define TEST_H 1

#include <core/parallel.h>

// namespace for testing various app features

namespace test
{
  inline void testParallel()
  {
    parallel::Queue<int> queue;

    if (queue.isEmpty())
    {
      fprintf_s(stdout, "queue is empty\n");
    }
    int a = 2;
    queue.push(a);
    int b = 3;
    queue.push(b);
    fprintf_s(stdout, "queue size: %llu, expected 2.\n", queue.size());
    int popped;
    if (queue.pop(popped)) fprintf_s(stdout, "%i\n", popped);
    fprintf_s(stdout, "queue size: %llu, expected 1.\n", queue.size());
    if (queue.pop(popped)) fprintf_s(stdout, "%i\n", popped);
    if (!queue.pop(popped)) fprintf_s(stdout, "can't pop, is empty\n");
    if (queue.isEmpty())
    {
      fprintf_s(stdout, "queue is empty\n");
    }

    fprintf_s(stdout, "Current tasks %llu\n", parallel::pool.numTasks());
    // parallel loops that push to the queue
    parallel::pool.pushTask([&queue]
    {
      for (int i = 0; i < 50; ++i)
      {
        queue.push(i);
      }
    });
    fprintf_s(stdout, "Submitted task 1\n");
    parallel::pool.pushTask([&queue]
    {
      for (int i = 50; i < 100; ++i)
      {
        queue.push(i);
      }
    });
    fprintf_s(stdout, "Submitted task 2\n");

    parallel::pool.waitForTasks();

    fprintf_s(stdout, "Queue size after tasks complete: %llu\n", queue.size());
    
    fprintf_s(stdout, "Num threads: %llu\n", parallel::pool.numThreads());
    auto t0 = sys_clock::now();
    
    parallel::pool.parallelFor(0ULL, 10000000000U, [](const size_t& i, const size_t& j)
    {
      for (size_t k = i; k < j; ++k)
      {
        k * k;
      }
    });
    fprintf_s(stdout, "Num tasks %llu\n", parallel::pool.numTasks());
    
    parallel::pool.waitForTasks();
    
    auto t1 = sys_clock::now();
    fprintf_s(stdout, "Parallel took %llu\n", std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count());
    
    
    t0 = sys_clock::now();
    
    for (size_t k = 0; k < 10000000000; ++k)
    {
      k * k;
    }
    t1 = sys_clock::now();

    fprintf_s(stdout, "Serial took %llu\n", std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count());
    
    /*
    fprintf_s(stdout, "square of %i is %i square of %i is %i square of %i is %i\n", 
      2, squares[2], 20, squares[20], 50, squares[50]);
    */
  }

  inline int launch(int id)
  {
    switch (id)
    {
    case 0:
      testParallel();
      break;
    default:
      break;
    }
    return 0;
  }
}


#endif // !TEST_H

