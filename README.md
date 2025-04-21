# Process-Hollowing
Basic program written in c that injects shellcode into a "legitimate" process (Process Hollowing)

I mention this in my code but this was not made to be used for anything, this is just an example.

The code extracts raw shell code from the binary (isell.bin)

# What is Process Hollowing?

```
Process Hollowing is a method used to avoid getting the attention of an anti-virus

This is used in malware.
It injects code into a legitimate/trusted process in order to avoid being detected.
```

# How To Compile
```
gcc -c loader.c -o loader.o
gcc main.c loader.o -o main.exe
```
