#pragma once

#include <math.h>

#include <deque>
#include <functional>
#include <iostream>

#if defined(__has_include)
#if __has_include(<format>)
#include <format>
#define FORMAT_SUPPORT
#endif
#endif

namespace search_tree_space {
template <typename KeyT = int>
class node {
 private:
  KeyT key;

 public:
  node* parent = nullptr;
  node* left = nullptr;
  node* right = nullptr;
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

template <typename KeyT = int, class Comparator = std::greater<KeyT>>
class tree {
 private:
  node<KeyT>* top;

  void change_size(node<KeyT>* node) {
    while (node != nullptr) {
      node->size = new_size(node);
      node = node->parent;
    }
  }

  void change_height(node<KeyT>* new_node) {
    if (new_node->left != nullptr) assign_height(new_node->left);

    if (new_node->right != nullptr) assign_height(new_node->right);

    assign_height(new_node);
    while (new_node != nullptr) {
      if (new_node->get_left_h() - new_node->get_right_h() ^ 2 > 1) {
        new_node = balance(new_node);
      }

      new_node = new_node->parent;
    }
  }

  size_t new_size(node<KeyT>* node) const {
    if (node == nullptr) return 0;

    size_t result = 1;

    if (node->left != nullptr) result += node->left->size;

    if (node->right != nullptr) result += node->right->size;

    return result;
  }

  node<KeyT>* balance(node<KeyT>* cur_node) {
    if (cur_node->get_left_h() > cur_node->get_right_h()) {
      node<KeyT>* left_tmp = cur_node->left;
      node<KeyT>* subtree = left_tmp->right;

      if (cur_node->parent != nullptr) {
        if (cur_node->parent->left == cur_node) {
          cur_node->parent->left = left_tmp;
        } else {
          cur_node->parent->right = left_tmp;
        }
      } else {
        top = left_tmp;
      }
      left_tmp->parent = cur_node->parent;

      left_tmp->right = cur_node;
      cur_node->parent = left_tmp;

      cur_node->left = subtree;

      if (subtree != nullptr) {
        subtree->parent = cur_node;
      }

      cur_node->size = new_size(cur_node);
      left_tmp->size = new_size(left_tmp);

      assign_height(subtree);
      assign_height(cur_node);
      assign_height(left_tmp);

      return left_tmp;
    }

    if (cur_node->get_left_h() < cur_node->get_right_h()) {
      node<KeyT>* right_tmp = cur_node->right;
      node<KeyT>* subtree = right_tmp->left;

      if (cur_node->parent != nullptr) {
        if (cur_node->parent->left == cur_node) {
          cur_node->parent->left = right_tmp;
        } else {
          cur_node->parent->right = right_tmp;
        }
      } else {
        top = right_tmp;
      }
      right_tmp->parent = cur_node->parent;

      right_tmp->left = cur_node;
      cur_node->parent = right_tmp;

      cur_node->right = subtree;

      if (subtree != nullptr) {
        subtree->parent = cur_node;
      }

      cur_node->size = new_size(cur_node);
      right_tmp->size = new_size(right_tmp);

      assign_height(subtree);
      assign_height(cur_node);
      assign_height(right_tmp);

      return right_tmp;
    }

    return cur_node;
  }

  void assign_height(node<KeyT>* cur_node) {
    if (cur_node == nullptr) {
      return;
    }
    if (cur_node->left != nullptr || cur_node->right != nullptr) {
      cur_node->height =
          std::max(cur_node->get_left_h(), cur_node->get_right_h()) + 1;
    } else {
      cur_node->height = 0;
    }
  }

  node<KeyT>* find(const KeyT& key) const {
    node<KeyT>* cur_elem = find_father(key);

    if (cur_elem->left->get_key() == key) return cur_elem->left;

    if (cur_elem->right->get_key() == key) return cur_elem->right;
    return nullptr;
  }

  node<KeyT>* find_father(const KeyT& key, Comparator comp) const {
    node<KeyT>* cur_elem = top;

    while (cur_elem != nullptr) {
      if (comp(key, cur_elem->get_key())) {
        cur_elem = cur_elem->right;
      } else if (comp(cur_elem->get_key(), key)) {
        cur_elem = cur_elem->left;
      } else {
        return cur_elem->parent;
      }
    }

    return nullptr;
  }

  int l_count(node<KeyT>* cur_node, const int a, Comparator comp = Comparator{}) const {
    if (cur_node == nullptr) {
      return 0;
    }

    int result = 0;

    while (cur_node != nullptr) {
      if (comp(cur_node->get_key(), a)) {
        ++result;
        if (cur_node->right != nullptr) result += cur_node->right->size;

        cur_node = cur_node->left;
        continue;
      }

      if (comp(a,cur_node->get_key())) {
        cur_node = cur_node->right;
        continue;
      }

      if (cur_node->get_key() == a) {
        ++result;
        if (cur_node->right != nullptr) result += cur_node->right->size;
      }

      break;
    }

    return result;
  }

  int r_count(node<KeyT>* cur_node, const int a, Comparator comp = Comparator{}) const {
    if (cur_node == nullptr) {
      return 0;
    }

    int result = 0;

    while (cur_node != nullptr) {
      if (comp(cur_node->get_key(), a)) {
        cur_node = cur_node->left;
        continue;
      }

      if (comp(a, cur_node->get_key())) {
        ++result;
        if (cur_node->left != nullptr) result += cur_node->left->size;

        cur_node = cur_node->right;
        continue;
      }

      if (cur_node->get_key() == a) {
        ++result;
        if (cur_node->left != nullptr) result += cur_node->left->size;
      }

      break;
    }

    return result;
  }

 public:
  bool insert(const KeyT& key, Comparator comp = Comparator{}) {
    if (top == nullptr) {
      top = new node<KeyT>(key);

      return true;
    }
    node<KeyT>* cur_elem = top;
    node<KeyT>* parent = cur_elem;
    bool inLeft = 0;

    // no dublicates in the tree!
    while (cur_elem != nullptr) {
      parent = cur_elem;
      if (comp(key, cur_elem->get_key())) {
        cur_elem = cur_elem->right;
        inLeft = 0;
      } else if (comp(cur_elem->get_key(), key)) {
        cur_elem = cur_elem->left;
        inLeft = 1;
      } else {
        return false;
      }
    }

    if (inLeft) {
      parent->left = new node<KeyT>(key);
      parent->left->parent = parent;
      change_size(parent->left);
      change_size(parent);

      assign_height(parent->left);
      change_height(parent);

      return true;
    } else {
      parent->right = new node<KeyT>(key);
      parent->right->parent = parent;
      change_size(parent->right);
      change_size(parent);

      assign_height(parent->right);
      change_height(parent);

      return true;
    }

    return false;
  }

  void print() const {
    std::deque<node<KeyT>*> all_nodes;
    all_nodes.push_front(top);

    for (int i = 0; !all_nodes.empty(); ++i) {
      node<KeyT>* cur_node = all_nodes.front();

      if (cur_node != nullptr) {
        all_nodes.push_back(cur_node->left);
        all_nodes.push_back(cur_node->right);
      }

      if (cur_node != nullptr)
#ifdef FORMAT_SUPPORT
        std::cout << std::format("{}({}) ", cur_node->get_key(),
                                 cur_node->height);
#else
        std::cout << cur_node->get_key() << "(" << cur_node->height << ") ";
#endif
      else
        std::cout << "null ";
      all_nodes.pop_front();
    }

    std::cout << std::endl;
  }

  int distance(const KeyT& a, const KeyT& b, Comparator comp = Comparator{}) const {
    if (comp(a, b) || top == nullptr) return 0;

    int result = 0;

    // find common subtree
    node<KeyT>* ptr_parent = top;
    do {
      if (comp(ptr_parent->get_key(), b)) {
        ptr_parent = ptr_parent->left;
        continue;
      } else if (comp(a, ptr_parent->get_key())) {
        ptr_parent = ptr_parent->right;
        continue;
      }

      break;
    } while (ptr_parent != nullptr);

    if (ptr_parent == nullptr) {
      return 0;
    }

    result += l_count(ptr_parent->left, a);
    result += r_count(ptr_parent->right, b);

    return result + 1;
  }

  ~tree() {
    std::deque<node<KeyT>*> all_nodes;
    all_nodes.push_front(top);

    while (!all_nodes.empty()) {
      node<KeyT>* cur_del_node = all_nodes.front();

      if (cur_del_node->left != nullptr) {
        all_nodes.push_back(cur_del_node->left);
      }

      if (cur_del_node->right != nullptr) {
        all_nodes.push_back(cur_del_node->right);
      }

      delete cur_del_node;
      all_nodes.pop_front();
    }
  }
};
}  // namespace search_tree_space
