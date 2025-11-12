#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    pid_t pid;
    FILE *tempFile;
    char buffer[100];

    printf("Parent Process: PID = %d\n", getpid());

    pid = fork();  // Create child process

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } 
    else if (pid == 0) {
        // Child Process
        printf("Child Process: PID = %d\n", getpid());
        char *args[] = {"ls", "-l", NULL};
        execvp(args[0], args);

        // If execvp fails
        perror("execvp failed");
        exit(1);
    } 
    else {
        // Parent Process
        wait(NULL);  // Wait for child to complete

        // Create a temporary file
        tempFile = fopen("tempfile.txt", "w+");
        if (tempFile == NULL) {
            perror("File creation failed");
            return 1;
        }

        fprintf(tempFile, "This is a test message from parent process (PID: %d).\n", getpid());
        fflush(tempFile); // Ensure data is written

        // Rewind to beginning of file
        rewind(tempFile);

        // Read file content
        printf("\nReading from temporary file:\n");
        while (fgets(buffer, sizeof(buffer), tempFile)) {
            printf("%s", buffer);
        }

        fclose(tempFile);
        printf("\nParent process completed successfully.\n");
    }

    return 0;
}

