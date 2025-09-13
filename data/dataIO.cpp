#include "dataIO.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
std::string readFile(const std::string &filePath)
{
    std::cout << "Reading file: " << filePath << std::endl;
    std::ifstream file(filePath);
    if (file.is_open())
    {
        std::cout << "File opened successfully." << std::endl;
    }
    else
    {
        std::cerr << "Warning: Could not open file: " << filePath << ". Assuming empty data." << std::endl;
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    if (rename(filePath.c_str(), (filePath + ".bak").c_str()))
    {
        std::cerr << "Error: Could not create backup file: " << filePath + ".bak" << std::endl;
        throw std::runtime_error("Could not create backup file: " + filePath + ".bak");
    }
    return content;
}
void writeFile(const std::string &filePath, const std::string &serializedData)
{
    const std::string tempFilePath = filePath + ".tmp";
    {
        std::ofstream file(tempFilePath, std::ios::binary);
        if (!file)
        {
            throw std::runtime_error("Could not create file: " + tempFilePath);
        }

        file << serializedData;
        file.flush();
        if (file.fail())
        {
            throw std::runtime_error("Write failed");
        }
    }
    if (std::rename(tempFilePath.c_str(), filePath.c_str()) != 0)
    {
        std::remove(tempFilePath.c_str());
        throw std::runtime_error("Could not rename temp file to: " + filePath);
    }
}
std::vector<passwordData> parse(const std::string &rawData)
{
    if (rawData.empty())
    {
        std::cout << "No data found to parse." << std::endl;
        return {};
    }
    std::vector<passwordData> vault;
    auto j = json::parse(rawData);
    for (const auto &item : j)
    {
        passwordData pd;
        pd.site = item.value("site", "");
        pd.userName = item.value("userName", "");
        pd.password = item.value("password", "");
        pd.updatedAt = item.value("updatedAt", "");
        vault.push_back(pd);
    }
    return vault;
}
std::string serialize(const std::vector<passwordData> &vault)
{
    json j = json::array();
    for (const auto &pd : vault)
    {
        j.push_back({{"site", pd.site},
                     {"userName", pd.userName},
                     {"password", pd.password},
                     {"updatedAt", pd.updatedAt}});
    }
    return j.dump(4); // Pretty print with an indent of 4 spaces
}