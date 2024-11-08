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

#include "tree.hpp"

namespace search_tree_space {
template <typename KeyT>
int range_query(tree<KeyT>& s, KeyT fst, KeyT snd) {
  if (fst > snd) {
    return 0;
  }

  using itt = typename tree<KeyT>::iterator;

  itt start = s.lower_bound(fst);
  itt fin = s.upper_bound(snd);

  return s.distance(start, fin);
}

template <typename KeyT> 
int my_distance(typename tree<KeyT>::iterator start, typename tree<KeyT>::iterator fin) {
  int result = 0;

  while (start != fin) {
    ++start;
    ++result;
  }

  return result;
}
}  // namespace search_tree_space