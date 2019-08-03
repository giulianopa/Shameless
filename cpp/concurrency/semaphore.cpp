#include <iostream>
#include <thread>
#include <array>
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;
using namespace std::chrono_literals;

const int N_THREADS = 6;

class Semaphore {
private:
  //! Simple mutex to access the count.
  std::mutex m_mux;

  // Count, to be checked modified in mutex (atomic compare and swap).
  int m_count;

  //! Condition variable for the compare and swap of the count.
  condition_variable m_cv;


public:
  /**
    Default constructor.
    \param count Semaphore count. Binary semaphore (count = 1) by default.
   */
  Semaphore(const int count = 1) : m_count(count) {}

  void wait() {
      unique_lock<mutex> lock(m_mux);
      m_cv.wait(lock, [&]{ return m_count > 0; });
      m_count--;
  }

  void signal() {
    lock_guard<mutex> guard(m_mux);
    m_count++;
    m_cv.notify_one();
  }
};

// Thread body
void body(const int id, Semaphore &semaphore) {
  this_thread::sleep_for(id * 100ms);
  semaphore.wait();
  cout << "[" << id << "] Aquired!" << endl;
  this_thread::sleep_for(id * 1s);
  cout.flush();
  semaphore.signal();
}

int main(int argc, char *argv[]) {
  array<thread, N_THREADS> ths;
  Semaphore semaphore(N_THREADS / 2);
  int id = 0;
  for (auto &th : ths) {
    th = thread(&body, id++, ref(semaphore));
  }

  // Could detach, honestly
  for (auto &th : ths) {
    th.join();
  }
  return 0;
}
