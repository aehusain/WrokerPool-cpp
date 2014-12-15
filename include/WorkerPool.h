/*
 * WorkerPool.h
 *
 * Copyright (C) <year>  <name of author>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Ata E Husain Bohra(ata.husain@hotmail.com)
 */

#pragma once

#include "Locks.h"
#include <functional>
#include <memory>
#include <condition_variable>
#include <boost/thread.hpp>
#include <deque>
#include <atomic>

using Task = std::function<void(void)>;

class AbstractWorkerPool
{
public:
    virtual Task waitForEvent() = 0;
};

/**
 * Worker
 *
 * Worker abstraction; spawns a thread and expose an
 * interface to perform submitted task
 */
class Worker
{
public:
    Worker(AbstractWorkerPool* ptr)
        : ptr_(ptr)
    {
        threadPtr_ = new boost::thread(&Worker::run, this);
    }
    // thread execution routine
    void run()
    {
        try {
            while (true) {
                Task t = ptr_->waitForEvent();
                if (t) t();
                else break;
            }
        } catch (...) {
            stopped_ = true;
            std::cout << threadPtr_->get_id() << " exiciting\n";
        }
    }
    void shutdown()
    { threadPtr_->interrupt(); }
    void join()
    { threadPtr_->join(); }
private:
    std::atomic<bool> stopped_;
    AbstractWorkerPool* ptr_;
    boost::thread *threadPtr_;
}; // Worker
using WorkerPtr = std::shared_ptr<Worker>;

// forward declaration
class WorkerPool;
using WorkerPoolPtr = std::shared_ptr<WorkerPool>;

/**
 * WorkerPool
 *
 * Abstraction to manage multiple workers;
 * expoeses Task queue and synchronize
 * publishing of task to the queue and their
 * execution by mutilple workers
 */
class WorkerPool : public AbstractWorkerPool
{
public:
    WorkerPool(int numWorkers);
   ~WorkerPool();
   // publish task to the worker pool
   void publish(const Task &t);
   // allow client to wait till all submitted task are done
   void waitForDrain();
   // API to synchronize workers with pool manager.
   virtual Task waitForEvent();
private:

    int numWorkers_;
    int cur_;
    bool stopped_;
    std::vector<WorkerPtr> workers_;
    std::deque<Task> tasks_;
    locks::BMutex mutex_;
    boost::condition_variable cv_;
    boost::condition_variable queueEmptyCV_;
}; // WorkerPool
