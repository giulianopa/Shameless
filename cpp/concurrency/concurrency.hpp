/*
* Copyright (C) 2019 Giuliano Pasqualotto (github.com/giulianopa)
* This code is licensed under MIT license (see LICENSE.txt for details)
*/
#pragma once
#include <mutex>
#include <condition_variable>


/**
  Implement a barrier using a condition_variable, returning true when count is 0.
 */
class Barrier {
private:
  int m_count = 0;
  std::mutex m_mux;
  std::condition_variable m_cv;

public:
  Barrier(const int num) : m_count(num) {}

  void wait();
};

/**
  The simplest semaphore implementation, using a condition_variable.
 */
class Semaphore {
private:
  //! Simple mutex to access the count.
  std::mutex m_mux;

  // Count, to be checked modified in mutex (atomic compare and swap).
  int m_count;

  //! Condition variable for the compare and swap of the count.
  std::condition_variable m_cv;

public:
  /**
    Default constructor.
    \param count Semaphore count. Binary semaphore (count = 1) by default.
   */
  Semaphore(const int count = 1) : m_count(count) {}

  void wait();

  void signal();
};
