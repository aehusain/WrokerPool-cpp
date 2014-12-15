/*
 * Stack.h
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

#include <exception>
#include "Locks.h"
#include <vector>
#include <algorithm>

class StackException : public std::exception
{
public:
    StackException(const std::string &errMsg)
        : errMsg_(errMsg)
    { }
    const char *what() const noexcept
    {
        return errMsg_.c_str();
    }
private:
    std::string errMsg_;
};

/**
 * Stack
 *
 * Implementation of Stack datastructure. Supported operations are:
 * 1. push - dynamically scales the storage if needed.
 * 2. pop
 * 3. peek
 */

template <typename T>
class Stack
{
public:
    Stack(int size)
        : cur_(0)
        , size_(size)
    {
        s_.resize(size_);
    }
    void push(T t)
    {
        locks::WriteLock _(mutex_);
        if (cur_ < size_) s_[cur_++] = t;
        else {
            // need to resize the stack
            std::vector<T> s;
            s.resize(2*size_);
            for (const auto &e : s_)
                s.push_back(e);
            s_.swap(s);
            s_.push_back(t);
        }
    }
    T pop()
    {
        locks::WriteLock _(mutex_);
        if (cur_ == 0) throw new StackException("Empty Stack!");
        T ret = s_[--cur_];

        return ret;
    }
    T peek()
    {
        locks::ReadLock _(mutex_);
        if (cur_ == 0) throw new StackException("Empty Stack!");
        T ret = s_[cur_];

        return ret;
    }

private:
    int cur_;
    int size_;
    std::vector<T> s_;
    locks::SharedMutex mutex_;
};


