/*
 * Locks.h
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

#include <atomic>
#include <boost/thread/shared_mutex.hpp>
#include <mutex>
#include <boost/thread/mutex.hpp>

/**
 * locks
 *
 * Unified namespace for various locking mechanism supported
 * by std, boost libraries as well semantics not supported
 * by both (for instance: SpinLock)
 */
namespace locks
{
// mutex variants
using Mutex = std::mutex;
using AutoLock = std::unique_lock<Mutex>;
// shared mutex variants
using SharedMutex = boost::shared_mutex;
using ReadLock = boost::shared_lock<SharedMutex>;
using WriteLock = boost::unique_lock<SharedMutex>;

// boost variants
using BMutex = boost::mutex;
using BAutoLock = boost::unique_lock<BMutex>;

class SpinLock
{
public:
    void lock()
    {
        while(lck.test_and_set(std::memory_order_acquire))
        {}
    }

    void unlock()
    {
        lck.clear(std::memory_order_release);
    }

private:
    std::atomic_flag lck = ATOMIC_FLAG_INIT;
};

} // locks
