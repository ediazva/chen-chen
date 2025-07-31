#pragma once
#include <iostream>

namespace logger {
  inline void info(const std::string& msg) {
    std::cout << "[INFO] " << msg << "\n";
  }

  inline void error(const std::string& msg) {
    std::cerr << "\033[1;31m[FATAL] " << msg << "\033[0m\n";
  }
} // namespace logger