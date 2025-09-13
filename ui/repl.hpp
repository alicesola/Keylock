#pragma once
#include <iostream>
#include <string>
#include "memTable.hpp"

bool accessSystem();
void menuLogic();
bool flushData(MemTable &data_);
bool findData(const MemTable &data_);
bool displaySite(const MemTable &data_);
bool manuallyFlush(MemTable &data_);
bool insertData(MemTable &data_);
bool deleteData(MemTable &data_);
bool updateData(MemTable &data_);