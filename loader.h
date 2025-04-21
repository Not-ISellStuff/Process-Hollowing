//////////////////////////
//  Shell Code Injector //
/////////////////////////

#ifndef LOADER_H
#define LOADER_H

#include <windows.h>

int load(unsigned char* shellcode, size_t size, DWORD pid);

#endif
