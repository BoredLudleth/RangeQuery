#pragma once

#include <iostream>
#include <iterator>
#include <set>

#if defined(__has_include)
#if __has_include(<format>)
#include <format>
#define FORMAT_SUPPORT
#endif
#endif

namespace common_space {
template <typename C, typename T>
int range_query(C& s, T fst, T snd) {
  if (fst > snd) {
    return 0;
  }

  using itt = typename C::iterator;

  itt start = s.lower_bound(fst);
  itt fin = s.upper_bound(snd);

  return std::distance(start, fin);
}
}  // namespace common_space
