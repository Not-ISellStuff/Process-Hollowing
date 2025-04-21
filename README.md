# Process-Hollowing
Basic program written in c that starts a "legitimate" process and injects shell code (Process Hollowing)

This was not made to be used for anything, this is just an example.

The code extracts the shell code from the binary (isell.bin)

# What is Process Hollowing?

```
Process Hollowing is a method used to avoid getting the attention of an anti-virus.
It starts a trusted process like notepad and makes it execute code

This is used in malware.
It injects code into a legitimate/trusted process in order to avoid being detected.
```

# How To Compile
```
gcc -c loader.c -o loader.o
gcc main.c loader.o -o main.exe
```
