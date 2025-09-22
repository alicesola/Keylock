#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

struct passwordData
{
    std::string site;
    std::string userName;
    std::string password;
    std::string updatedAt;
};
std::string readFile(const std::string &filePath);
void writeFile(const std::string &filePath, const std::string &serializedData);
void writeFile(const std::string &filePath, const std::vector<unsigned char> &data);
std::vector<passwordData> parse(const std::string &rawData);
std::string serialize(const std::vector<passwordData> &vault);