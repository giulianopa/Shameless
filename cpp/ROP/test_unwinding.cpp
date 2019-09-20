// Modified from sample found on the Internet
#include <iostream>

// If enabled, f2 won'd be removed from the stack
//#define HANDLER_ON_F2

using namespace std;

// A sample function f1() that throws an int exception
void f1() {
  cout << "f1() Start" << endl;
  throw 100;
  cout << "f1() End" << endl; // Not executed
}

// Another sample function f2() that calls f1()
void f2() {
  cout << "f2() Start " << endl;
#ifdef HANDLER_ON_F2
  try {
    f1();
  }
  catch (int i) {
    cout << "Exception: "<< i << endl;
  }
#else
  f1();
#endif
  cout << "f2() End" << endl; // Not executed
}

// Another sample function f3() that calls f2() and handles exception thrown by f1()
void f3() {
  cout << "f3() Start" << endl;
  try {
    f2();
  }
  catch (int i) {
    cout << "Exception: "<< i << endl;
  }
  cout << "f3() End" << endl;
}

/*
  When exception is thrown in f1, the stack is searched linerly to find an
  exception handler, removing stack function entries along the way.
 */
int main() {
  f3();
  return 0;
}
