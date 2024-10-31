#ifndef INPUTSYS_H
#define INPUTSYS_H

#include <string>
#include <iostream>

std::string* handleInput(int argc, char* argv[]);
std::string* getPrefs();

// Helper functions
std::string input(std::string question);

#endif
