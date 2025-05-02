Apparently `make` checks the last time has been modified, so it doesn't have to compile files that have already been compiled with no change, this is useful when compiling large projects...

# Entering Raw Mode
We use the `unistd.h` library to use `read()` and `STDIN_FILENO` ot be ablt to read 1 byte from the standard library until there is no more bytes to read, the `read()` function returns the number of bytes that it read, then returns 0 when it reaches the end of file.