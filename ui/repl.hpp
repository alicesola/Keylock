#pragma once
#include <string>
#include <limits>
#include "memTable.hpp"
#include "crypto.hpp"
#include "timeFormat.hpp"
#include <iostream>
#include <cstdlib>
#include <filesystem>
#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

void waitForEnter();
bool accessSystem();
void menuLogic();
bool initialize();