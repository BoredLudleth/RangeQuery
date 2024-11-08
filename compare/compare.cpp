#include <chrono>
#include <vector>

#include "range_query_tree.hpp"
#include "range_query.hpp"
#include "tree.hpp"

int main() {
  using namespace search_tree_space;
  using namespace common_space;

  std::vector<int> data;
  int a = 0, b = 0;
  char command = 0;
  bool fin = true;

  while (fin) {
    std::cin >> command;
    switch (command) {
      case 'k':
        std::cin >> a;
        data.push_back('k');

        if (!std::cin.good()) {
          std::cout << "Invalid input!\n";
          return 1;
        }

        data.push_back(a);
        command = 0;
        break;
      case 'q':
        data.push_back('q');
        std::cin >> a >> b;

        if (!std::cin.good()) {
          std::cout << "Invalid input!\n";
          return 1;
        }

        data.push_back(a);
        data.push_back(b);
        command = 0;
        break;

      default:
        fin = false;
        break;
    }
  }

  volatile int length = 0;

  auto begin = std::chrono::steady_clock::now();

  tree<int> my_tree{};

  for (auto it = data.begin(); it != data.end(); ++it) {
    switch (*it) {
      case 'k': {
        ++it;
        my_tree.insert(*it);

        break;
      }
      case 'q': {
        ++it;
        length = search_tree_space::range_query(my_tree, *it, *(std::next(it)));
        ++it;
        break;
      }

      default:
        break;
    }
  }
  auto end = std::chrono::steady_clock::now();
  auto elapsed_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
#ifdef FORMAT_SUPPORT
  std::cout << std::format("AVL_tree-Time:{} s\n",
                           static_cast<float>(elapsed_ms.count()) / 1000);
#else
  std::cout << "Time: " << static_cast<float>(elapsed_ms.count()) / 1000
            << " s\n";
#endif
  begin = std::chrono::steady_clock::now();
  std::set<int> thug;

  for (auto it = data.begin(); it != data.end(); ++it) {
    switch (*it) {
      case 'k': {
        ++it;

        thug.insert(*it);
        break;
      }
      case 'q': {
        ++it;

        length = range_query(thug, *it, *(std::next(it)));
        ++it;
        break;
      }

      default: {
        break;
      }
    }
  }

  auto end_set = std::chrono::steady_clock::now();
  auto elapsed_ms_set =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_set - begin);
#ifdef FORMAT_SUPPORT
  std::cout << std::format("Set-Time:{} s\n",
                           static_cast<float>(elapsed_ms_set.count()) / 1000);
#else
  std::cout << "Time: " << static_cast<float>(elapsed_ms.count()) / 1000
            << " s\n";
#endif
  return 0;
}
