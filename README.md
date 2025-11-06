Lunix Programming Assignment

Question 1: ELF File Disassembler

Description:
A C program that disassembles ELF (Executable and Linkable Format) files using the objdump command.
It converts binary executable files into readable assembly text format.

How it works:

Reads all executable files from the /bin directory.

Runs the objdump -d command on each file.

Stores the disassembly result in a .txt file.

Compile & Run:

gcc -o elf_disassembler elf_disassembler.c
./elf_disassembler

Question 2: Mini Shell

Description:
A simple Unix-like shell program that executes basic Linux commands.
It supports built-in commands such as exit, and system commands using execvp().

How it works:

Reads user input from the terminal.

Splits the input into command and arguments.

Executes commands using system calls.

Compile & Run:

gcc -o mini_shell mini_shell.c
./mini_shell

Question 3: Race Condition (Thread Synchronization)

Description:
A multithreaded program that increments a shared counter safely using a mutex to prevent race conditions.

How it works:

Creates 5 threads, each incrementing a shared counter 10,000 times.

Uses pthread_mutex_lock() and pthread_mutex_unlock() for synchronization.

Displays the total number of threads and the final counter value.

Compile & Run:

gcc -o race_condition race_condition.c -lpthread
./race_condition

Question 4: Command-Line Diary Manager (with Shared Library)

Description:
A diary manager that encrypts and decrypts personal journal entries using a shared library (.so).

How it works:

Part A: Shared library (libdiary.so) provides encrypt() and decrypt() functions using a key of 4.

Part B: Main program (diary_manager.c) lets the user add and view encrypted diary entries after password verification.

Compile & Run:

# Build shared library
gcc -fPIC -shared -o libdiary.so diary_lib.c

# Build main executable
gcc -o diary_manager diary_manager.c -L. -ldiary

# Run
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./diary_manager


Author: Hirwa Walid
Language: C
Platform: Linux/Ubuntu
Tools Used: GCC Compiler, pthreads, objdump, Python3
