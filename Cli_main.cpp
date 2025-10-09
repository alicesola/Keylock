#include "repl.hpp"
#include <iostream>

int main()try
{
    if (accessSystem())
    {
        menuLogic();
    }
    return 0;
}catch(const std::exception&ex){
    writeLogFile(ex);
    return EXIT_FAILURE;
}
