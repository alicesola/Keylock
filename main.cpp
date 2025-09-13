#include "ui/repl.hpp"
#include <iostream>

int main()
{
    if (accessSystem())
    {
        menuLogic();
    }
    return 0;
}
