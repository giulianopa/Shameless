/*
* Copyright (C) 2019 Giuliano Pasqualotto (github.com/giulianopa)
* This code is licensed under MIT license (see LICENSE.txt for details)
*/
#include "concurrency.hpp"

using namespace std;

/*
 * Barrier
 */

void Barrier::wait() {
  unique_lock<mutex> lock(m_mux);
  if (--m_count)
    m_cv.wait(lock, [&]{ return m_count == 0; });
  else
    m_cv.notify_all();
}

/*
 * Semaphore
 */

void Semaphore::wait() {
  unique_lock<mutex> lock(m_mux);
  m_cv.wait(lock, [&]{ return m_count > 0; });
  m_count--;
}

void Semaphore::signal() {
  lock_guard<mutex> guard(m_mux);
  m_count++;
  m_cv.notify_one();
}
