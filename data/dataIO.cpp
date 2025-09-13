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
    return content;
}