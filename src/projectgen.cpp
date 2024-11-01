#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <pwd.h>
#include <unistd.h>
#include <vector>
#include <algorithm>

#include "projectgen.h"
#include "rollback.h"

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
    exit(1);
  }

  try {
    fs::create_directories(projectDirectory);
    
    std::vector<std::string> subDirs = {"src", "include", "build"};
    for (const auto& subDir : subDirs) {
      fs::create_directories(projectDirectory + subDir + "/");
    }

    if (!doesDirExist(projectDirectory) || !doesDirExist(projectDirectory + "src/") || !doesDirExist(projectDirectory + "include/") || !doesDirExist(projectDirectory + "build/")) {
      std::cerr << "Error generating project directory" << std::endl;
      fatalErrorRollback(projectDirectory);
      exit(1);
    }
  } catch (const fs::filesystem_error& e) {
    std::cerr << "Error generating project directories: " << e.what() << std::endl;
    fatalErrorRollback(projectDirectory);
    exit(1);
  }
}

void generateFiles(std::string* prefs) {
  std::string dir = (prefs[1][0] == '~') ? convertShortDir(prefs[1]) : prefs[1];

  dir += prefs[0] + "/";

  if (prefs[4] == "n") {
    generateFile("/usr/local/share/ps/templates/CMakeListsTemplate.txt", dir, "CMakeLists.txt", prefs);
  }

  if (prefs[2] == "c++") {
    generateFile("/usr/local/share/ps/templates/mainCpp.txt", dir + "src/", "main.cpp");
    generateFile("/usr/local/share/ps/templates/gitignoreCpp.txt", dir, ".gitignore");
  } else if (prefs[2] == "c") {
    generateFile("/usr/local/share/ps/templates/mainC.txt", dir + "src/", "main.c");
    generateFile("/usr/local/share/ps/templates/gitignoreC.txt", dir, ".gitignore");
  } else {
    std::cerr << "Language not supported" << prefs[2] << std::endl;
    fatalErrorRollback(dir);
    exit(1);
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
    return "";
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

bool containsSpaces(const std::string& str) {
  return str.find(' ') != std::string::npos;
}

std::string replacePlaceholders(const std::string& templateStr, const std::string& projectName, const std::string& language, const std::string& version, const std::string& path) {
  std::string result = templateStr;

  std::string name;
  if (containsSpaces(projectName)) {
    name = projectName;
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
  } else {
    name = projectName;
  }

  std::string lang;
  if (language == "c++") {
    lang = "CXX";
  } else if (language == "c") {
    lang = "C";
  } else {
    std::cerr << "Language is not supported: " << language << std::endl;
    fatalErrorRollback(path);
    exit(1);
  }

  std::string fileEnding;
  if (language == "c++") {
    fileEnding = "cpp";
  } else if (language == "c") {
    fileEnding = language;
  } else {
    std::cerr << "Language is not supported: " << language << std::endl;
    fatalErrorRollback(path);
    exit(1);
  }
  
  size_t pos = 0;

  while ((pos = result.find("{{PROJECT_NAME}}", pos)) != std::string::npos) {
    result.replace(pos, std::string("{{PROJECT_NAME}}").length(), name);
    pos += name.length();
  }

  pos = 0;
  while ((pos = result.find("{{LANGUAGE}}", pos)) != std::string::npos) {
    result.replace(pos, std::string("{{LANGUAGE}}").length(), lang);
    pos += lang.length();
  }

  pos = 0;
  while ((pos = result.find("{{LANGUAGE_VERSION}}", pos)) != std::string::npos) {
    result.replace(pos, std::string("{{LANGUAGE_VERSION}}").length(), version);
    pos += version.length();
  }

  pos = 0;
  while ((pos = result.find("{{FILE}}", pos)) != std::string::npos) {
    result.replace(pos, std::string("{{FILE}}").length(), fileEnding);
    pos += fileEnding.length();
  }

  return result;
}


void generateFile(const std::string& templatePath, const std::string& projectPath, std::string file, std::string* prefs) {
  if (prefs != nullptr) {
    std::string templateContent = readTemplate(templatePath);

    std::string populatedTemplate = replacePlaceholders(templateContent, prefs[0], prefs[2], prefs[3], projectPath);

    std::ofstream File(projectPath + file);
    if (!File) {
      std::cerr << "Error generating file: " << file << std::endl;
      fatalErrorRollback(projectPath);
      exit(1);
    }

    File << populatedTemplate;
    File.close();
    return;
  } else {
    std::ofstream File(projectPath + file);
    if (!File) {
      std::cerr << "Error generating file: " << file << std::endl;
      fatalErrorRollback(projectPath);
      exit(1);
    }

    File << readTemplate(templatePath);
    File.close();
    return;
  }
}
