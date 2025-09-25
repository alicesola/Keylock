#pragma once
#include <iostream>
#include <string>
#include "memTable.hpp"
#include "crypto.hpp"

void waitForEnter();
bool accessSystem();
void menuLogic();
bool flushData(MemTable &data_);
bool findData(const MemTable &data_);
bool displaySite(const MemTable &data_);
bool manuallyFlush(MemTable &data_);
bool insertData(MemTable &data_);
bool deleteData(MemTable &data_);
bool updateData(MemTable &data_);
bool initialize();
std::string getpass_unix(const char *prompt);
std::string getpass_win(const char *prompt);