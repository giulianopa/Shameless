#include <iostream>
#include <thread>
#include <array>
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;
using namespace std::chrono_literals;

const int N_THREADS = 6;

/**
  Implement a barrier using a condition_variable, returning true when count is 0.
  Useless with C++20 (can't wait).
 */
class Barrier {
private:
  int count = 0;
  std::mutex m_mux;
  condition_variable condition;

public:
  Barrier(const int num) : count(num) {}

  void wait() {
    unique_lock<mutex> lock(m_mux);
    if (--count)
      condition.wait(lock, [&]{ return count == 0; });
    else
      condition.notify_all();
  }
};

// Thread body
void body(const int id, Barrier &barrier) {
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
  Barrier barrier(N_THREADS);
  int id = 0;
  for (auto &th : ths) {
    th = thread(&body, id++, ref(barrier));
  }

  // Could detach, honestly
  for (auto &th : ths) {
    th.join();
  }
  return 0;
}
