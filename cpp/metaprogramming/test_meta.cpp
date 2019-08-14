/*
* Copyright (C) 2018 Giuliano Pasqualotto (github.com/giulianopa)
* This code is licensed under MIT license (see LICENSE.txt for details)
*/
#include <iostream>
#include "meta.hpp"

using namespace std;

int main(int argc, char** argv) {
  const int VALUE = 8;
  cout << "Fibonacci(" << VALUE << ") = " << Fibonacci<VALUE>::value << endl;
  cout << "Factorial(" << VALUE << ") = " << Factorial<VALUE>::value << endl;
  cout << "List<1, 2, 3, 4, 5, 6, 7, 8>::sum = " << CreateList<int, VALUE>::type::sum << endl;
  return 0;
}
