#include "dataIO.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
std::string readFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    if(rename(filePath.c_str(), (filePath + ".bak").c_str())){
        throw std::runtime_error("Could not create backup file: " + filePath + ".bak");
    }
    return content;
}
void writeFile(const std::string &filePath, const std::string &serializedData){
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    file << serializedData;
    file.close();
}
