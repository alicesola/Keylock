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
    if (rename(filePath.c_str(), (filePath + ".bak").c_str()))
    {
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
    if(std::rename(tempFilePath.c_str(), filePath.c_str()) != 0)
    {
        std::remove(tempFilePath.c_str());
        throw std::runtime_error("Could not rename temp file to: " + filePath);
    }
}
std::vector<passwordData> parse(const std::string &rawData)
{
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