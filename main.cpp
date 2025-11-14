#include "source/include/console.hpp"

int main()
{
    cinter::Console console;
    console.write(L"test\n");
    console.write(L"тест\n");
    console.write(L"tête\n");
    console.write(L"τεςτ\n");

    return 0;
}
