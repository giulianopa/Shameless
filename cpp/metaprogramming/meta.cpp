#include <iostream>


//-----------
// Fibonacci
//-----------

template <int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template <>
struct Fibonacci<0> {
    static constexpr int value = 0;
};
template <>
struct Fibonacci<1> {
    static constexpr int value = 1;
};


//-----------
// Factorial
//-----------

template <int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static constexpr int value = 1;
};


//------
// List
//------

template <typename T, T E, T... Elems>
struct List {
  static constexpr T sum = E + List<T, Elems...>::sum;
};

template <typename T, T E>
struct List<T, E> {
  static constexpr T sum = E;
};

// Helper, to expand to List<T, 1, 2, 3, ..., MAX>
template<typename T, size_t M, T... Elems>
struct CreateList : CreateList<T, M - 1, M, Elems...> {
};

template<typename T, T... Elems>
struct CreateList<T, 1, Elems...> {
  typedef List<T, 1, Elems...> type;
};

template<typename T, size_t M>
struct CreateList<T, M> : CreateList<T, M - 1, M> {
};


using namespace std;

int main(int argc, char** argv) {
  const int VALUE = 8;
  cout << "Fibonacci(" << VALUE << ") = " << Fibonacci<VALUE>::value << endl;
  cout << "Factorial(" << VALUE << ") = " << Factorial<VALUE>::value << endl;
  cout << "List<1, 2, 3, 4, 5, 6, 7, 8>::sum = " << CreateList<int, VALUE>::type::sum << endl;
  return 0;
}
