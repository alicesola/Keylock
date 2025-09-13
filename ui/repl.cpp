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


