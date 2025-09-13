#include "repl.hpp"
#include <iostream>

bool accessSystem(const std::string& password) {
    std::cout << "=== Login ===\n";
    std::cout << "Enter password: ";
    const std::string correctPassword = "Gensokyo";
    if (password == correctPassword) {
        std::cout << "Login successful!\n";
        return true;
    }
    else if(password == "hint"){
        std::cout << "Where you want to see";
        return false;
    }
     else {
        std::cout << "Login failed. Incorrect password.\n";
        return false;
    }
}
void menuLogic() {
    MemTable data("/data/data.txt");
    std::cout << "=== Main Menu ===\n";
    std::cout << "1. View Data\n";
    std::cout << "2. Flush Data\n";
    std::cout << "3. Find Data\n";
    std::cout << "4. Exit\n";
    std::cout << "9. Initialize Data\n";
    std::cout << "=================\n";
    int choice=0;
    std::cin >> choice;

    switch (choice) {
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
            exitApplication();
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            menuLogic();
            break;
    }
}
bool displaySite(const MemTable& data_) {
    const auto& allData = data_.getAll();
    if (allData.empty()) {
        std::cout << "No data available.\n";
        return;
    }
    std::cout << "=== Stored Data ===\n";
    for (const auto& pd : allData) {
        std::cout << "Site: " << pd.site << "\n";
    }
    std::cout << "===================\n";
    return true;
}


