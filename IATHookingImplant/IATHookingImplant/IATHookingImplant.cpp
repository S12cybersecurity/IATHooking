#include <iostream>
#include <Windows.h>

int main(){
    std::cout << "Hello World!\n";
    DWORD baseAddress = (DWORD)GetModuleHandle(NULL);
    std::cout << "Base Address: " << baseAddress << std::endl;
    std::getchar();
}

