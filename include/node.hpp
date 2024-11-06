#pragma once

namespace search_tree_space {
template <typename KeyT = int>
class node {
 private:
  KeyT key;

 public:
  std::weak_ptr<node> parent;
  std::shared_ptr<node> left = nullptr;
  std::shared_ptr<node> right = nullptr;
  size_t size = 0;

  int height = 0;
  node(const KeyT& key = 0) : key(key) {}

  KeyT get_key() const { return key; }

  bool is_right_null() const { return right == nullptr; }

  bool is_left_null() const { return left == nullptr; }

  int get_left_h() const {
    if (left == nullptr) return 0;

    return left->height;
  }

  int get_right_h() const {
    if (right == nullptr) return 0;

    return right->height;
  }
};
};  // namespace search_tree_space
