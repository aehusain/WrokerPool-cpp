WorkerPool-cpp

Lightweight threadpool implementation in C++.

Abstraction provides flexibility to spawn worker threads backed by a queue.
Task are submitted to queue via "publish" interface, on publish one of the
worker thread will pick up the task and execute it.
