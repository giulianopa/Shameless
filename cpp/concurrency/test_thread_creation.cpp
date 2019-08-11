#include <thread>

using namespace std;

// Measure execution time over multiple runs and average
int main(int argc, char** argv)
{
  for (volatile int i = 0; i < 1000000; i++)
    thread([](){}).detach();
  return 0;
}
