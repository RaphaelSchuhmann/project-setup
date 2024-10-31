#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <pwd.h>
#include <unistd.h>
#include <vector>

#include "projectgen.h"

namespace fs = std::filesystem;

void generateFolders(const std::string& projectName, const std::string& inputProjectDirectory) {
  std::string projectDirectory;

  if (inputProjectDirectory[0] == '~') {
    projectDirectory = convertShortDir(inputProjectDirectory);
  } else {
    projectDirectory = inputProjectDirectory;
  }

  projectDirectory += projectName + "/";

  if (doesDirExist(projectDirectory)) {
    std::cerr << "Error: Project directory already exists: " << projectDirectory << std::endl;
  }

  try {
    fs::create_directories(projectDirectory);
    
    std::vector<std::string> subDirs = {"src", "include", "build"};
    for (const auto& subDir : subDirs) {
      fs::create_directories(projectDirectory + subDir + "/");
    }

    if (!doesDirExist(projectDirectory) || !doesDirExist(projectDirectory + "src/") || !doesDirExist(projectDirectory + "include/") || !doesDirExist(projectDirectory + "build/")) {
      std::cerr << "Error generating project directory" << std::endl;
      exit(1);
    }
  } catch (const fs::filesystem_error& e) {
    std::cerr << "Error generating project directories: " << e.what() << std::endl;
    exit(1);
  }
}

void generateFiles(std::string* prefs) {
  std::string dir = convertShortDir(prefs[1]);
  std::cout << prefs[3];

  dir += prefs[0] + "/";

  if (prefs[4] == "n") {
    generateFile("/home/raphael/Development/project-setup/templates/CMakeListsTemplate.txt", dir, "CMakeLists.txt");
  }

  if (prefs[3] == "c++") {
    generateFile("/home/raphael/Development/project-setup/templates/mainCpp.txt", dir + "src/", "main.cpp");
  } else if (prefs[3] == "c") {
    generateFile("/home/raphael/Development/project-setup/templates/mainC.txt", dir + "src/", "main.c");
  }
}

bool doesDirExist(const std::string& path) {
  return fs::exists(path) && fs::is_directory(path);
}

std::string convertShortDir(const std::string& dir) {
  const char* home = getpwuid(getuid())->pw_dir;
  return std::string(home) + dir.substr(1);
}

std::string readTemplate(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Error opening template file: " + path;
    exit(1);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void generateFile(const std::string& templatePath, const std::string& projectPath, std::string file) {
  std::ofstream File(projectPath + file);
  if (!File) {
    std::cerr << "Error generating file: " << file << std::endl;
    exit(1);
  }

  File << readTemplate(templatePath);
  File.close();
  return;
}
