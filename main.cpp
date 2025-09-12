#include "ui/repl.hpp"
#include <iostream>

int main()
{
    displayLoginUI();

    int count = 0;
    std::string password;
    while (count != 3)
    {
        std::cin >> password;
        if (login(password))
        {
            std::cout << "Welcome to the application!\n";
        }
        else
        {
            std::cout << "Access denied.\n";
            count++;
        }
    }
    // TODO:data validation;
    int mainOption;
    while (mainOption != 3)
    {
        std::cout << "1. Login\n2. Exit\nChoose an option: ";
        std::cin >> mainOption;
    }

    return 0;
}
