#include <string>
#include <iostream>
#include <cstdlib>

#include "inputsys.h"

std::string* handleInput(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: pset create" << std::endl;
    exit(1);
  }

  std::string command = argv[1];

  if (command == "create") {
    std::string* projectPrefs = getPrefs();

    for (int i = 0; i < 5; i++) {
      if (projectPrefs[i] == "") {
        switch (i) {
          case 0:
            std::cerr << "Project name can not be empty!" << std::endl;
            break;
          case 1:
            std::cerr << "Project directory can not be empty!" << std::endl;
            break;
          case 2:
            std::cerr << "Programming language can not be empty!" << std::endl;
            break;
          case 3:
            std::cerr << "Language version can not be empty!" << std::endl;
            break;
          case 4:
            projectPrefs[4] = "n";
            break;
          default:
            std::cerr << "Unexpected error!" << std::endl;
            break;
        }

        exit(1);
      }
    }
    
    return projectPrefs;
  }
}

std::string* getPrefs() {
  std::string* projectPrefs = new std::string[5];

  projectPrefs[0] = input("Project name: ");

  projectPrefs[1] = input("Project directory: ");

  projectPrefs[2] = input("Programming language (c, c++): ");

  projectPrefs[3] = input("Language version: ");

  projectPrefs[4] = input("Custom compiler (y/n): ");

  return projectPrefs;
}

// Helper functions

std::string input(std::string question) {
  std::string input;
  std::cout << question;
  std::getline(std::cin, input);

  if (std::cin.fail()) {
    std::cerr << "Input failed!" << std::endl;
    exit(1);
  }

  return input;
}
