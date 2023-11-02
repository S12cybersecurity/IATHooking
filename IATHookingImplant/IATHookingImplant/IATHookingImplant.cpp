#include <iostream>
#include <Windows.h>

using namespace std;

IMAGE_IMPORT_DESCRIPTOR* getImportTable() {
    DWORD baseAddress = (DWORD)GetModuleHandle(NULL);
    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*) baseAddress;
    IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)((DWORD)baseAddress + (*dosHeader).e_lfanew);
    IMAGE_OPTIONAL_HEADER optionalHeader = (IMAGE_OPTIONAL_HEADER)(*ntHeader).OptionalHeader;
    IMAGE_DATA_DIRECTORY importTableDesc = optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    IMAGE_IMPORT_DESCRIPTOR* importTable = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)baseAddress + importTableDesc.VirtualAddress);
    return importTable;
}

int main(){
    
}

