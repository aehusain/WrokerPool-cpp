/*
 * WorkerPool.cpp
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

#include "WorkerPool.h"

WorkerPool::WorkerPool(int numWorkers)
    : numWorkers_(numWorkers)
    , cur_(0)
    , stopped_(false)
{
    // initialize the WorkerPool
    for (int i = 0; i < numWorkers; ++i) {
        WorkerPtr ptr = std::make_shared<Worker>(this);
        workers_.push_back(ptr);
    }
    std::cout << "Allocated workerpool with threads: " << numWorkers_ << "\n";
}

WorkerPool::~WorkerPool()
{
    stopped_ = true;
    {
        locks::BAutoLock lock(mutex_);
        cv_.notify_all();
    }
    for (auto &w : workers_) w->join();
    for (auto &w : workers_) w->shutdown();
    workers_.clear();
}

void
WorkerPool::waitForDrain()
{
    locks::BAutoLock lock(mutex_);
    if (tasks_.size() > 0)
        queueEmptyCV_.wait(lock);
}

void
WorkerPool::publish(const Task &t)
{
    locks::BAutoLock lock(mutex_);
    tasks_.push_back(t);
    cv_.notify_one();
}

Task
WorkerPool::waitForEvent()
{
    Task t = nullptr;
    locks::BAutoLock lock(mutex_);
    while (tasks_.empty() && !stopped_) {
        if (tasks_.empty())
            queueEmptyCV_.notify_all();
        cv_.wait(lock);
    }
    if (!stopped_) {
        t = tasks_.front();
        tasks_.pop_front();
    }

    return t;
}

