#include <string>
#include <iostream>

#include "inputsys.h"
#include "projectgen.h"

void printPrefs(std::string* prefs) {
  for (int i = 0; i < 5; i++) {
    std::cout << prefs[i] << std::endl;
  }
}

int main(int argc, char* argv[]) {
  std::string* prefs = handleInput(argc, argv);
  // printPrefs(prefs);

  generateFolders(prefs[0], prefs[1]);
  generateFiles(prefs);

  return 0;
}
