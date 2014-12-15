/*
 * testStack.cpp
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

#include "Stack.h"
#include "WorkerPool.h"
#include <iostream>

/**
 * Test program simulating; multiple threads consuming
 * stack datastructure
 */
int
main(int argc, char **argv)
{
    // allocate a worker pool to similuate multi-thread enviornment
    WorkerPool workers(4);
    int numOperations = 100;
    Stack<int> s(5);

    for (int i = 0; i < numOperations; ++i) {
        workers.publish([&s, i]() {
                s.push(i);
                std::cout << ":push: " << i << "\n";
                std::cout << ":pop: " << s.pop() << "\n";
          });
    }

    workers.waitForDrain();
    std::cout << "All task done!\n";

    return 0;
}

