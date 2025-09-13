#include "repl.hpp"
#include "timeFormat.hpp"
#include <iostream>

bool accessSystem(const std::string &password)
{
    std::cout << "=== Login ===\n";
    std::cout << "Enter password: ";
    const std::string correctPassword = "Gensokyo";
    if (password == correctPassword)
    {
        std::cout << "Login successful!\n";
        return true;
    }
    else if (password == "hint")
    {
        std::cout << "Where you want to see";
        return false;
    }
    else
    {
        std::cout << "Login failed. Incorrect password.\n";
        return false;
    }
}
void menuLogic()
{
    MemTable data("/data/data.json");
    std::cout << "=== Main Menu ===\n";
    std::cout << "1. View Data\n";
    std::cout << "2. Flush Data\n";
    std::cout << "3. Find Data\n";
    std::cout << "4. Insert Data\n";
    std::cout << "5. Delete Data\n";
    std::cout << "6. Update Data\n";
    std::cout << "7. Exit\n";
    std::cout << "=================\n";
    int choice = 0;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        displaySite(data);
        break;
    case 2:
        flushData(data);
        break;
    case 3:
        findData(data);
        break;
    case 4:
        insertData(data);
        break;
    case 5:
        deleteData(data);
        break;
    case 6:
        updateData(data);
        break;
    case 7:
        exitApplication();
        break;
    default:
        std::cout << "Invalid choice. Please try again.\n";
        menuLogic();
        break;
    }
}
bool displaySite(const MemTable &data_)
{
    const auto &allData = data_.getAll();
    if (allData.empty())
    {
        std::cout << "No data available.\n";
        return false;
    }
    std::cout << "=== Stored Data ===\n";
    for (const auto &pd : allData)
    {
        std::cout << "Site: " << pd.site << "\n";
    }
    std::cout << "===================\n";
    return true;
}
bool findData(const MemTable &data)
{
    std::cout << "Enter site to find: ";
    std::string site;
    std::cin >> site;
    const passwordData *pd = data.find(site);
    if (pd)
    {
        std::cout << "Site: " << pd->site << "\n";
        std::cout << "Username: " << pd->userName << "\n";
        std::cout << "Password: " << pd->password << "\n";
        std::cout << "Last Updated: " << pd->updatedAt << "\n";
        return true;
    }
    else
    {
        std::cout << "No entry found for site: " << site << "\n";
        return false;
    }
}
bool flushData(MemTable &data_)
{
    try
    {
        data_.flushToDisk();
        std::cout << "Data flushed to disk successfully.\n";
        return true;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error flushing data: " << e.what() << "\n";
        return false;
    }
}
bool insertData(MemTable &data_)
{
    passwordData pd;
    std::cout << "Enter site: ";
    std::cin >> pd.site;
    std::cout << "Enter username: ";
    std::cin >> pd.userName;
    std::cout << "Enter password: ";
    std::cin >> pd.password;
    pd.updatedAt = currentISO8601();
    try
    {
        data_.insert(pd);
        std::cout << "Data inserted successfully.\n";
        return true;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error inserting data: " << e.what() << "\n";
        return false;
    }
}