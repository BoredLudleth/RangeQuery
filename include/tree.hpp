#pragma once

#include <climits>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>

#if defined(__has_include)
#if __has_include(<format>)
#include <format>
#define FORMAT_SUPPORT
#endif
#endif

#include "node.hpp"

namespace search_tree_space {
template <typename KeyT = int, class Comparator = std::greater<KeyT>>
class tree {
 private:
  using node_t = node<KeyT>;
  using data_it = std::list<node_t>::iterator;

  node_t* top = nullptr;
  std::list<node_t> data;

  node_t* iter_to_ptr(const data_it& it) const { return &(*it); }

  void assign_height(node_t* cur_node) {
    if (cur_node == nullptr) {
      return;
    }

    if (cur_node->left != nullptr || cur_node->right != nullptr)
      cur_node->height =
          std::max(cur_node->get_left_h(), cur_node->get_right_h()) + 1;
    else
      cur_node->height = 0;
  }

  void change_height(node_t* cur_node) {
    if (cur_node->left != nullptr) assign_height(cur_node->left);

    if (cur_node->right != nullptr) assign_height(cur_node->right);

    assign_height(cur_node);

    while (cur_node != top->parent) {
      if ((cur_node->get_left_h() - cur_node->get_right_h()) ^ 2 > 1)
        cur_node = balance(cur_node);

      cur_node = cur_node->parent;
    }
  }

  void change_size(node_t* node) {
    while (node != nullptr) {
      node->update_size();

      node = node->parent;
    }
  }

  node_t* balance(node_t* cur_node) {
    if (cur_node->get_left_h() > cur_node->get_right_h()) {
      node_t* left_tmp = cur_node->left;
      node_t* subtree = left_tmp->right;

      if (cur_node->parent->left == cur_node)
        cur_node->parent->left = left_tmp;
      else
        cur_node->parent->right = left_tmp;

      left_tmp->parent = cur_node->parent;

      left_tmp->right = cur_node;
      cur_node->parent = left_tmp;

      cur_node->left = subtree;

      if (cur_node == top) {
        top = left_tmp;
        top->parent = &(data.front());
        // data.begin()->right = top;
      }

      if (subtree != nullptr) {
        subtree->parent = cur_node;
      }

      cur_node->update_size();
      left_tmp->update_size();

      assign_height(subtree);
      assign_height(cur_node);
      assign_height(left_tmp);

      return left_tmp;
    }

    if (cur_node->get_left_h() < cur_node->get_right_h()) {
      node_t* right_tmp = cur_node->right;
      node_t* subtree = right_tmp->left;

      if (cur_node->parent->left == cur_node)
        cur_node->parent->left = right_tmp;
      else
        cur_node->parent->right = right_tmp;

      right_tmp->parent = cur_node->parent;

      right_tmp->left = cur_node;
      cur_node->parent = right_tmp;

      cur_node->right = subtree;

      if (cur_node == top) {
        top = right_tmp;
        top->parent = &(data.front());
        // data.begin()->right = top;
      }

      if (subtree != nullptr) {
        subtree->parent = cur_node;
      }

      cur_node->update_size();
      right_tmp->update_size();

      assign_height(subtree);
      assign_height(cur_node);
      assign_height(right_tmp);

      return right_tmp;
    }

    return cur_node;
  }

 public:
  bool insert(const KeyT& key, const Comparator& comp = Comparator{}) {
    if (top == nullptr) {
      data.push_back(INT_MAX);
      data.push_back(key);

      top = &data.back();
      top->parent = &(data.front());
      data.begin()->right = top;

      return true;
    }

    node_t* cur_node = top;
    node_t* parent = top;  //?

    bool inLeft = 0;

    while (cur_node != nullptr) {
      parent = cur_node;
      if (comp(key, cur_node->get_key())) {
        cur_node = cur_node->right;
        inLeft = 0;
      } else if (comp(cur_node->get_key(), key)) {
        cur_node = cur_node->left;
        inLeft = 1;
      } else {
        return false;
      }
    }

    if (inLeft) {
      data.push_back(key);
      parent->left = &data.back();
      parent->left->parent = parent;

      change_size(parent->left);
      change_size(parent);

      assign_height(parent->left);
      change_height(parent);

      return true;
    } else {
      data.push_back(key);
      parent->right = &data.back();
      parent->right->parent = parent;

      change_size(parent->right);
      change_size(parent);

      assign_height(parent->right);
      change_height(parent);

      return true;
    }
    return false;
  }

  class iterator : public std::iterator_traits<node_t> {
   private:
    Comparator comp;
    const node_t* ptr;

   public:
    iterator(const node_t* node_ptr, Comparator comp = Comparator{})
        : ptr(node_ptr), comp(comp) {}

    const node_t& operator*() const { return *ptr; };

    const node_t* operator->() const { return ptr; }

    bool operator==(const iterator& other) const {
      if (ptr == other.ptr) {
        return true;
      }

      return false;
    }

    bool operator!=(const iterator& other) const {
      return !(operator==(other));
    }

    iterator& operator++() {
      if (ptr == nullptr) {
        return *this;
      }

      if (ptr->right != nullptr) {
        ptr = ptr->right;

        return to_lowest();
      }

      const node_t* tmp = ptr;

      do {
        ptr = ptr->parent;
      } while (comp(tmp->get_key(), ptr->get_key()));

      return *this;
    }

    iterator operator++(int) {
      iterator tmp = *this;
      operator++();

      return tmp;
    }

    iterator& operator--() {
      if (ptr == nullptr) {
        return *this;
      }

      if (ptr->left != nullptr) {
        ptr = ptr->left;

        return to_highest();
      }

      if (ptr->parent == nullptr) {
        ptr = ptr->right;

        return to_highest();
      }

      const node_t* tmp = ptr;

      do {
        ptr = ptr->parent;
      } while (comp(ptr->get_key(), tmp->get_key()));

      return *this;
    }

    iterator operator--(int) {
      iterator tmp = *this;
      --this;

      return tmp;
    }
    iterator& to_lowest() & {
      while (ptr->left != nullptr) {
        ptr = ptr->left;
      }

      return *this;
    }

    iterator& to_highest() {
      while (ptr->right != nullptr) {
        ptr = ptr->right;
      }

      return *this;
    }
  };

  iterator begin() const {
    iterator start(top);
    start.to_lowest();

    return start;
  }

  iterator end() const {
    iterator finish(top->parent);
    return finish;
  }

  iterator lower_bound(KeyT key, const Comparator& comp = Comparator{}) const {
    node_t* current_ptr = top;
    node_t* result_ptr = top->parent;

    while (current_ptr != nullptr) {
      if (!comp(key, current_ptr->get_key())) {
        if (comp(result_ptr->get_key(), current_ptr->get_key()))
          result_ptr = current_ptr;

        current_ptr = current_ptr->left;
        continue;
      }

      if (comp(key, current_ptr->get_key())) {
        current_ptr = current_ptr->right;
        continue;
      }
    }

    iterator result(result_ptr);
    return result;
  }

  iterator upper_bound(KeyT key) const {
    iterator result = lower_bound(key);
    if (result->get_key() == key) {
      ++result;
    }
    return result;
  }

  int distance(iterator start, iterator fin,
               const Comparator& comp = Comparator{}) const {
    int result = 0;

    if (start == fin || top == nullptr) {
      return 0;
    }

    const node_t* low = &*start;
    const node_t* high = &*fin;

    node_t* cur_node = top;

    do {
      if (comp(cur_node->get_key(), high->get_key())) {
        cur_node = cur_node->left;
        continue;
      } else if (comp(low->get_key(), cur_node->get_key())) {
        cur_node = cur_node->right;
        continue;
      }

      break;
    } while (cur_node != nullptr);

    node_t* tmp = cur_node;
    cur_node = cur_node->left;

    while (cur_node != nullptr) {
      if (comp(cur_node->get_key(), low->get_key())) {
        ++result;
        if (cur_node->right != nullptr) result += cur_node->right->size;

        cur_node = cur_node->left;
        continue;
      }

      if (comp(low->get_key(), cur_node->get_key())) {
        cur_node = cur_node->right;
        continue;
      }

      if (const_cast<const node_t*>(cur_node) == low) {
        ++result;
        if (cur_node->right != nullptr) result += cur_node->right->size;
      }

      break;
    }

    if (!comp(low->get_key(), tmp->get_key()) &&
        comp(high->get_key(), tmp->get_key()))
      ++result;

    cur_node = tmp->right;

    while (cur_node != nullptr) {
      if (comp(cur_node->get_key(), high->get_key())) {
        cur_node = cur_node->left;
        continue;
      }

      if (comp(high->get_key(), cur_node->get_key())) {
        ++result;
        if (cur_node->left != nullptr) result += cur_node->left->size;

        cur_node = cur_node->right;
        continue;
      }

      if (const_cast<const node_t*>(cur_node) == high) {
        if (cur_node->left != nullptr) result += cur_node->left->size;
      }

      break;
    }

    return result;
  }

  void print() const {
    for (iterator it = begin(); it != end(); ++it) {
#ifdef FORMAT_SUPPORT
      std::cout << std::format("{} {} ", it->get_key(), it->get_size());
#else
      std::cout << it->get_key() << " ";
#endif
    }

    std::cout << std::endl;
  }
};
}  // namespace search_tree_space