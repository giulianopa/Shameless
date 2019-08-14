/*
* Copyright (C) 2018 Giuliano Pasqualotto (github.com/giulianopa)
* This code is licensed under MIT license (see LICENSE.txt for details)
*/
#include <iostream>
#include "meta.hpp"
#include "traits.hpp"

using namespace std;

class AlgoA : GenericAlgo<double, int> {
public:
  void preprocess() override { cout << __PRETTY_FUNCTION__ << endl; }
  int process(const double in) override {
    cout << __PRETTY_FUNCTION__ << "(" << in << ")" << endl;
    return (int)in;
  }
  void postprocess() override { cout << __PRETTY_FUNCTION__ << endl; }
};

class AlgoB : GenericAlgo<double, int> {
public:
  int process(const double in) override {
    cout << __PRETTY_FUNCTION__ << "(" << in << ")" << endl;
    return (int)in * 10;
  }
};

class AlgoC : GenericAlgo<float, int> {
public:
  int process(const float in) override {
    cout << __PRETTY_FUNCTION__ << "(" << in << ")" << endl;
    return (int)in * 100;
  }
  void postprocess() override { cout << __PRETTY_FUNCTION__ << endl; }
};

template<>
struct is_an_algo<double, int, AlgoA> : _is_an_algo<double, int, AlgoA> {
  static constexpr bool with_preproc = false;
  static constexpr bool with_postproc = true;
};

int main(int argc, char** argv) {
  const int VALUE = 8;
  cout << "Fibonacci(" << VALUE << ") = " << Fibonacci<VALUE>::value << endl;
  cout << "Factorial(" << VALUE << ") = " << Factorial<VALUE>::value << endl;
  cout << "List<1, 2, 3, 4, 5, 6, 7, 8>::sum = " << CreateList<int, VALUE>::type::sum << endl;
  AlgoA algoA;
  AlgoB algoB;
  AlgoC algoC;
  double arg = 10.0;
  int res = 0;
  algo_invoker(algoA, arg, res);
  cout << res << endl;
  algo_invoker(algoB, arg, res);
  cout << res << endl;
  algo_invoker(algoC, arg, res);
  cout << res << endl;
  return 0;
}
