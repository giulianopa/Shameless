/*
* Copyright (C) 2019 Giuliano Pasqualotto (github.com/giulianopa)
* This code is licensed under MIT license (see LICENSE.txt for details)
*/
#include <iostream>
#include <thread>
#include <array>
#include <chrono>
#include "concurrency.hpp"

using namespace std;
using namespace std::chrono_literals;

const int N_THREADS = 6;

void body(const int id, Semaphore &semaphore, Barrier &barrier) {
  this_thread::sleep_for(id * 100ms);
  semaphore.wait();
  cout << "[" << id << "] Aquired!" << endl;
  this_thread::sleep_for(id * 1s);
  cout.flush();
  semaphore.signal();
  this_thread::sleep_for(id * 100ms);
  cout << "Starting " << id << endl;
  this_thread::sleep_for(id * 1s);
  cout << "Waiting " << id << endl;
  barrier.wait();
  cout << "Terminating " << id << endl;
  cout.flush();
}

int main(int argc, char *argv[]) {
  array<thread, N_THREADS> ths;
  Semaphore semaphore(N_THREADS / 2);
  Barrier barrier(N_THREADS);
  int id = 0;
  for (auto &th : ths) {
    th = thread(&body, id++, ref(semaphore), ref(barrier));
  }

  // Could detach, honestly
  for (auto &th : ths) {
    th.join();
  }
  return 0;
}
