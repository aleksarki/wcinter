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

    while (true)
    {
        cinter::Dword size;
        std::cin >> size;
        console.cursorInfo({ size, true });
    }

    return 0;
}
