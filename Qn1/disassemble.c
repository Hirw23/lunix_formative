#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path_to_ELF_executable>\n", argv[0]);
        return 1;
    }

    char *elf_path = argv[1];

    // Check if file exists and is executable
    if (access(elf_path, F_OK) != 0) {
        perror("Error: File does not exist");
        return 1;
    }

    if (access(elf_path, X_OK) != 0) {
        perror("Error: File is not executable");
        return 1;
    }

    // Extract the base filename
    char *base = strrchr(elf_path, '/');
    if (base)
        base++;
    else
        base = elf_path;

    // Create output filename in current directory
    char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "%s_disassembly.txt", base);

    // Prepare objdump command
    char command[512];
    snprintf(command, sizeof(command), "objdump -d %s > %s", elf_path, output_filename);

    // Execute command
    int status = system(command);
    if (status != 0) {
        fprintf(stderr, "Error: Failed to run objdump command.\n");
        return 1;
    }

    printf("Disassembly completed successfully.\nOutput saved to: %s\n", output_filename);
    return 0;
}

