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
    parallel::ThreadPool pool;

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

    fprintf_s(stdout, "Current tasks %llu\n", pool.numTasks());
    // parallel loops that push to the queue
    pool.pushTask([&queue] 
    {
      for (int i = 0; i < 50; ++i)
      {
        queue.push(i);
      }
    });
    fprintf_s(stdout, "Submitted task 1\n");
    pool.pushTask([&queue]
    {
      for (int i = 50; i < 100; ++i)
      {
        queue.push(i);
      }
    });
    fprintf_s(stdout, "Submitted task 2\n");

    pool.waitForTasks();

    fprintf_s(stdout, "Queue size after: %llu\n", queue.size());
    
    while(queue.pop(a))
    {
      fprintf_s(stdout, "%i\n", a);
    }
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

