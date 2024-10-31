#ifndef PROJECTGEN_H
#define PROJECTGEN_H

#include <string>
#include <filesystem>

void generateFolders(const std::string& projectName, const std::string& projectDirectory);
void generateFiles(std::string* prefs);

bool doesDirExist(const std::string& path);
std::string convertShortDir(const std::string& projectDir);
std::string readTemplate(const std::string& filePath);
void generateFile(const std::string& templatePath, const std::string& projectPath, std::string file);

#endif // !PROJECTGEN_H
