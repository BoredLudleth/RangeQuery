#pragma once

#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <iterator>

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
  std::shared_ptr<node<KeyT>> top;
  std::shared_ptr<node<KeyT>> end_node;

  void change_size(std::shared_ptr<node<KeyT>> node) {
    while (node != end_node) {
      node->size = new_size(node);
      node = node->parent.lock();
    }
  }

  void change_height(std::shared_ptr<node<KeyT>> new_node) {
    if (new_node->left != nullptr) assign_height(new_node->left);

    if (new_node->right != nullptr) assign_height(new_node->right);

    assign_height(new_node);
    while (new_node != end_node) {
      if (new_node->get_left_h() - new_node->get_right_h() ^ 2 > 1) {
        new_node = balance(new_node);
      }

      new_node = new_node->parent.lock();
    }
  }

  size_t new_size(std::shared_ptr<node<KeyT>> node) const {
    if (node == nullptr) return 0;

    size_t result = 1;

    if (node->left != nullptr) result += node->left->size;

    if (node->right != nullptr) result += node->right->size;

    return result;
  }

  std::shared_ptr<node<KeyT>> balance(std::shared_ptr<node<KeyT>> cur_node) {
    if (cur_node->get_left_h() > cur_node->get_right_h()) {
      std::shared_ptr<node<KeyT>> left_tmp = cur_node->left;
      std::shared_ptr<node<KeyT>> subtree = left_tmp->right;

      if (cur_node->parent.lock() != end_node) {
        if (cur_node->parent.lock()->left == cur_node) {
          cur_node->parent.lock()->left = left_tmp;
        } else {
          cur_node->parent.lock()->right = left_tmp;
        }
      } else {
        left_tmp->parent = top->parent;
        top = left_tmp;
        top->parent.lock()->right = top;
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
      std::shared_ptr<node<KeyT>> right_tmp = cur_node->right;
      std::shared_ptr<node<KeyT>> subtree = right_tmp->left;

      if (cur_node->parent.lock() != end_node) {
        if (cur_node->parent.lock()->left == cur_node) {
          cur_node->parent.lock()->left = right_tmp;
        } else {
          cur_node->parent.lock()->right = right_tmp;
        }
      } else {
        right_tmp->parent = top->parent;
        top = right_tmp;
        top->parent.lock()->right = top;
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

  void assign_height(std::shared_ptr<node<KeyT>> cur_node) {
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

  std::shared_ptr<node<KeyT>> find(const KeyT& key) const {
    std::shared_ptr<node<KeyT>> cur_elem = find_father(key);

    if (cur_elem->left->get_key() == key) return cur_elem->left;

    if (cur_elem->right->get_key() == key) return cur_elem->right;
    return nullptr;
  }

  std::shared_ptr<node<KeyT>> find_father(const KeyT& key, const Comparator& comp = Comparator{}) const {
    std::shared_ptr<node<KeyT>> cur_elem = top;

    while (cur_elem != nullptr) {
      if (comp(key, cur_elem->get_key())) {
        cur_elem = cur_elem->right;
      } else if (comp(cur_elem->get_key(), key)) {
        cur_elem = cur_elem->left;
      } else {
        return cur_elem->parent.lock();
      }
    }

    return nullptr;
  }

  int l_count(std::shared_ptr<node<KeyT>> cur_node, const int a, const Comparator& comp = Comparator{}) const {
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

  int r_count(std::shared_ptr<node<KeyT>> cur_node, const int a, const Comparator& comp = Comparator{}) const {
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
  class iterator : public std::iterator<std::bidirectional_iterator_tag, node<KeyT>>{
    public:
      std::shared_ptr<node<KeyT>> ptr;

      iterator(std::shared_ptr<node<KeyT>> node_ptr) : ptr(node_ptr) {}

        node<KeyT>& operator*() const {
          return *ptr;
        };

        std::shared_ptr<node<KeyT>> operator->() const {
          return ptr;
        }

        bool operator==(const iterator &other) const {
          if (ptr == other.ptr) {
            return true;
          }

          return false;
        }

        bool operator!=(const iterator &other) const {
          return !(operator==(other));
        }

        iterator& operator++() {
          if (ptr == nullptr) {
            return *this;
          }

          if (ptr->right != nullptr){
            ptr = ptr->right;

            return to_lowest();
          }

          std::shared_ptr<node<KeyT>> tmp = ptr;
          
          do {
            ptr = ptr->parent.lock();
          } while (ptr->get_key() < tmp->get_key());

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

          if (ptr->left != nullptr){
            ptr = ptr->left;

            return to_highest();
          }

          if (ptr->parent.lock() == nullptr) {
            ptr = ptr->right;

            return to_highest();
          }

          std::shared_ptr<node<KeyT>> tmp = ptr;
          
          do {
            ptr = ptr->parent.lock();
          } while (ptr->get_key() > tmp->get_key());

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
          while(ptr->right != nullptr) {
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
    iterator finish(top->parent.lock());
    
    return finish;
  }

  iterator lower_bound(KeyT key) const {
    std::shared_ptr<node<KeyT>> current_ptr = top;
    std::shared_ptr<node<KeyT>> result_ptr = top->parent.lock();

    while(current_ptr != nullptr) {
      if (current_ptr->get_key() >= key) {
        if (current_ptr->get_key() < result_ptr->get_key())
          result_ptr = current_ptr;

        current_ptr = current_ptr->left;
        continue;
      }

      if (current_ptr->get_key() < key) {
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
      result++;
    }
    return result;
  }

  bool insert(const KeyT& key, const Comparator& comp = Comparator{}) {
    if (top == nullptr) {
      top = std::make_shared<node<KeyT>>(key);
      end_node = std::make_shared<node<KeyT>>(INT_MAX);
      top->parent = end_node;
      end_node->right = top;

      return true;
    }
    std::shared_ptr<node<KeyT>> cur_elem = top;
    std::shared_ptr<node<KeyT>> parent = cur_elem;
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
      parent->left = std::make_shared<node<KeyT>>(key);
      parent->left->parent = parent;

      change_size(parent->left);
      change_size(parent);

      assign_height(parent->left);
      change_height(parent);

      return true;
    } else {
      parent->right = std::make_shared<node<KeyT>>(key);
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
    std::deque<std::shared_ptr<node<KeyT>>> all_nodes;
    all_nodes.push_front(top);

    for (int i = 0; !all_nodes.empty(); ++i) {
      std::shared_ptr<node<KeyT>> cur_node = all_nodes.front();

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

  int distance(const KeyT& a, const KeyT& b, const Comparator& comp = Comparator{}) const {
    if (comp(a, b) || top == nullptr) return 0;

    int result = 0;

    // find common subtree
    std::shared_ptr<node<KeyT>> ptr_parent = top;
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
};
}  // namespace search_tree_space
