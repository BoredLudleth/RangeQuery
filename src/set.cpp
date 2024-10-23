#include "set_range_query.hpp"

#ifndef NDEBUG
#include <chrono>
#endif

int main() {
  using namespace set_space;
  std::set<int> thug;
  char command = 0;

#ifndef NDEBUG
  auto begin = std::chrono::steady_clock::now();
#endif

  int a = 0, b = 0;
  while (true) {
    std::cin >> command;
    switch (command) {
      case 'k':
        std::cin >> a;

        if (!std::cin.good()) {
          std::cout << "Invalid input!\n";
          return 1;
        }

        thug.insert(a);
        command = 0;
        break;
      case 'q':
        std::cin >> a >> b;

        if (!std::cin.good()) {
          std::cout << "Invalid input!\n";
          return 1;
        }

        std::cout << range_query(thug, a, b) << " ";
        command = 0;
        break;

      default:
#ifndef NDEBUG
        std::cout << '\n';
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
#ifdef FORMAT_SUPPORT
        std::cout << std::format("Time:{} s\n",
                                 static_cast<float>(elapsed_ms.count()) / 1000);
#else
        std::cout << "Time: " << static_cast<float>(elapsed_ms.count()) / 1000
                  << " s\n";
#endif
#endif

        return 0;
        break;
    }
  }
}