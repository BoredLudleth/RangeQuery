#pragma once

namespace search_tree_space {
template <typename KeyT = int>
class node {
 private:
  KeyT key;

 public:
  node* parent = nullptr;
  node* left   = nullptr;
  node* right  = nullptr;

  int size = 1;

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

  int get_size() const {
    return size;
  }

  void update_size() {
    size = 1;

    if (left != nullptr)
      size += left->get_size();

    if (right != nullptr)
      size += right->get_size();
  }
};
};  // namespace search_tree_space
