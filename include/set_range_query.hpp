#pragma once

#include <iostream>
#include <iterator>
#include <set>

namespace set_space {
template <typename C, typename T>
int range_query(const C& s, T fst, T snd) {
  if (fst > snd) {
    return 0;
  }

  using itt = typename C::iterator;

  itt start = s.lower_bound(fst);
  itt fin = s.upper_bound(snd);

  return std::distance(start, fin);
}
}
