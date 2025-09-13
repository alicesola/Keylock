#pragma once
#include<iostream>
#include<string>
#include"data/memTable.hpp"

void exitApplication();
bool accessSystem();
void menuLogic();
bool displaySite(const MemTable& data_);
bool manuallyFlush(MemTable& data_);