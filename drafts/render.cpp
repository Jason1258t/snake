#include <iostream>
#include <ostream>
#include <vector>

template <typename T>
std::ostream &operator<<(std::ostream &o, std::vector<T> vec) {
  for (T i : vec) {
    o << i << ' ';
  }
  o << std::endl;
  return o;
}

