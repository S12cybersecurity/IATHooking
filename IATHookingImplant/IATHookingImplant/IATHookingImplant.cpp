#include <iostream>
#include <Windows.h>

using namespace std;

int hookedMessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType){
    MessageBoxW(NULL, L"Hooked", L"Hooked", MB_OK);
    return 0;
}

PIMAGE_IMPORT_DESCRIPTOR getImportTable() {
    LPVOID baseAddress = GetModuleHandleA(NULL);
    PIMAGE_DOS_HEADER dosHeaders = (PIMAGE_DOS_HEADER)baseAddress;
    PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)baseAddress + dosHeaders->e_lfanew);
    IMAGE_DATA_DIRECTORY importTableDesc = ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    PIMAGE_IMPORT_DESCRIPTOR importTable = NULL;
    importTable = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD_PTR)baseAddress + importTableDesc.VirtualAddress);
    return importTable;
}

int iatHooking(string funcName, PIMAGE_IMPORT_DESCRIPTOR importTable, LPVOID hookFunction) {
    /*
        Import Table represents a list of the DLLs that are imported by the executable, inside the import table we have
        the DLL name and the functions that are imported from that DLL.

        Interesting fields:

        OriginalFirstThunk: Import Name Table (INT) Pointer
        FirstThunk: Import Address Table (IAT) Pointer
        Name: DLL Name
    */
    LPVOID baseAddress = GetModuleHandleA(NULL);
    while (importTable->Name != NULL) {
		PIMAGE_THUNK_DATA INT = (PIMAGE_THUNK_DATA)((DWORD_PTR)baseAddress + importTable->OriginalFirstThunk);
		PIMAGE_THUNK_DATA IAT = (PIMAGE_THUNK_DATA)((DWORD_PTR)baseAddress + importTable->FirstThunk);
        while (INT->u1.AddressOfData != NULL) {
			PIMAGE_IMPORT_BY_NAME importByName = (PIMAGE_IMPORT_BY_NAME)((DWORD_PTR)baseAddress + INT->u1.AddressOfData);
			//cout << "Function Name: " << importByName->Name << endl;
            if (string(importByName->Name) == funcName) {
				cout << "Found " << funcName << endl;
				DWORD oldProtect;
				VirtualProtect(&IAT->u1.Function, sizeof(DWORD_PTR), PAGE_READWRITE, &oldProtect);
				IAT->u1.Function = (DWORD_PTR)hookFunction;
			}
			INT++;
			IAT++;
		}
		importTable++;
	}
	return 0;   
}

int main(){    
    PIMAGE_IMPORT_DESCRIPTOR importTable = getImportTable();
    iatHooking("MessageBoxA", importTable, (LPVOID)hookedMessageBox);
    cout << "Hook setted press a key..." << endl;
    getchar();
    MessageBoxA(NULL, "Hello World", "IATHookingImplant", MB_OK);
    return 0;
}

