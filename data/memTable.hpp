#pragma once

#include <iostream>
#include "dataIO.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "crypto.hpp"

class MemTable
{
public:
    explicit MemTable(const std::string &filePath, const std::string &password) : filePath(filePath), password(password)
    {
        std::cout << "Loading data from disk..." << std::endl;
        loadFromDisk();
    }
    passwordData *find(const std::string &site)
    {
        for (auto &p : data_)
        {
            if (p.site == site)
            {
                return &p;
            }
        }
        return nullptr;
    }
    const passwordData *find(const std::string &site) const
    {
        for (const auto &p : data_)
        {
            if (p.site == site)
            {
                return &p;
            }
        }
        return nullptr;
    }
    void insert(const passwordData &pd)
    {
        if (find(pd.site))
        {
            throw std::runtime_error("Entry for site already exists: " + pd.site);
        }
        data_.push_back(pd);
    }
    void update(const passwordData &pd)
    {
        if (passwordData *p = find(pd.site))
        {
            *p = pd;
        }
        else
        {
            throw std::runtime_error("No entry found for site: " + pd.site);
        }
    }
    bool remove(const std::string &site)
    {
        auto it = std::remove_if(data_.begin(), data_.end(), [&](const passwordData &pd)
                                 { return pd.site == site; });
        if (it == data_.end())
        {
            return false;
        }
        data_.erase(it, data_.end());
        return true;
    }
    const std::vector<passwordData> &getAll() const
    {
        return data_;
    }
    void flushToDisk(std::string filePath)
    {
        const std::string serializedData = serialize(data_);
        writeFile(filePath,
                  encrypt(serializedData, password));
    }
    ~MemTable() = default;

private:
    std::string filePath;
    std::string password;
    std::vector<passwordData> data_;
    void loadFromDisk()
    {

        const std::string rawData = decrypt(readBin(filePath), password);
        std::cout << "Data loaded successfully. Entries count: " << (rawData.empty() ? 0 : parse(rawData).size()) << std::endl;
        data_ = parse(rawData);
    }
};
