#include <iostream>

#include "tree.hpp"

#ifndef NDEBUG
#include <chrono>
#endif

int main() {
  using namespace search_tree_space;

  char command = 0;
  int first_key = 0;

  tree<int>* my_tree = nullptr;

#ifndef NDEBUG
  auto begin = std::chrono::steady_clock::now();
#endif

  int a = 0, b = 0;
  while (true) {
    std::cin >> command;

    static bool is_allocated = 0;
    switch (command) {
      case 'k':
        std::cin >> a;

        if (!is_allocated) {
          my_tree = new tree<int>(a);
          is_allocated = 1;
        } else {
          my_tree->insert(a);
        }

        if (!std::cin.good()) {
          std::cout << "Invalid input!\n";

          if (my_tree != nullptr) {
            delete my_tree;
          }

          return 1;
        }

        command = 0;
        break;
      case 'q':
        std::cin >> a >> b;

        if (!std::cin.good()) {
          std::cout << "Invalid input!\n";

          if (my_tree != nullptr) {
            delete my_tree;
          }

          return 1;
        }
        if (my_tree != nullptr) {
          std::cout << my_tree->distance(a, b) << " ";
        } else {
          std::cout << 0 << " ";
        }
        command = 0;
        break;

      default:
        if (my_tree != nullptr) {
          delete my_tree;
        }

#ifndef NDEBUG
        std::cout << std::endl;
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        std::cout << "Time: " << static_cast<float>(elapsed_ms.count()) / 1000
                  << " s\n";
#endif

        return 0;
        break;
    }
  }
}