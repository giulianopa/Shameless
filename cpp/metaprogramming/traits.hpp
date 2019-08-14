/*
* Copyright (C) 2019 Giuliano Pasqualotto (github.com/giulianopa)
* This code is licensed under MIT license (see LICENSE.txt for details)
*/
#pragma once
#include "meta.hpp"

// Traits (abstract) class.
template<typename I, typename O>
class GenericAlgo {
public:
  virtual void preprocess() {}
  virtual O process(const I in) = 0;
  virtual void postprocess() {}
};

template<typename I, typename O, class Derived>
struct _is_an_algo {
  static constexpr bool value = std::is_base_of<GenericAlgo<I, O>, Derived>::value;
};

template<typename I, typename O, class Derived>
struct is_an_algo : _is_an_algo<I, O, Derived> {
  static constexpr bool with_preproc = _is_an_algo<I, O, Derived>::value && false;
  static constexpr bool with_postproc = _is_an_algo<I, O, Derived>::value && false;
};

template<typename O, bool has_pre, bool has_post>
struct algo_selector {
  template<typename T, typename I>
  static O invoke(T& obj, I in) {
    return obj.process(in);
  }
};

template<typename O>
struct algo_selector<O, true, false> {
  template<typename T, typename I>
  static O invoke(T& obj, I in) {
    obj.preprocess();
    return obj.process(in);
  }
};

template<typename O>
struct algo_selector<O, false, true> {
  template<typename T, typename I>
  static O invoke(T &obj, I in) {
    O out = obj.process(in);
    obj.postprocess();
    return out;
  }
};

template<typename O>
struct algo_selector<O, true, true> {
  template<typename T, typename I>
  static O invoke(T &obj, I in) {
    obj.preprocess();
    O out = obj.process(in);
    obj.postprocess();
    return out;
  }
};

template<typename T, typename I, typename O>
void algo_invoker(T &obj, I in, O &out) {
  out = algo_selector<O, is_an_algo<I, O, T>::with_preproc, is_an_algo<I, O, T>::with_postproc>::invoke(obj, in);
}
