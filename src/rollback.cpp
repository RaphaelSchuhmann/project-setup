#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "rollback.h"

namespace fs = std::filesystem;

void fatalErrorRollback(const std::string& dir) {
  fs::path dirPath = dir;
  std::cerr << "Removing project directory..." << std::endl;
  try {
    if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
      fs::remove_all(dirPath);
      std::cout << "Removed project directory!" << std::endl;
      return;
    } else {
      std::cerr << "Project directory does not exist!" << dirPath << std::endl;
      exit(1);
    }
  } catch (const fs::filesystem_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    exit(1);
  }
}
