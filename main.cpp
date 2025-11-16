#include "source/include/console.hpp"
#include "source/include/structs.hpp"

#include <iostream>

int main()
{
    cinter::Console console;
    
    console.write(L"test\n");
    console.write(L"тест\n");
    console.write(L"tête\n");
    console.write(L"τεςτ\n");

    cinter::ScreenBufferInfo info = console.screenBufferInfo();
    std::cout << info.cursorPosition.y;


    return 0;
}
