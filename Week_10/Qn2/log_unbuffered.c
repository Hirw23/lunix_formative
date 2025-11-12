#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

int main() {
    int fd = open("unbuffered_log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("File open failed");
        return 1;
    }

    char buffer[128];
    time_t now;
    ssize_t bytes_written;

    for (int i = 0; i < 1000000; i++) {
        now = time(NULL);
        int len = snprintf(buffer, sizeof(buffer), "Entry %d: %s", i + 1, ctime(&now));
        bytes_written = write(fd, buffer, len);

        if (bytes_written != len) {
            perror("Write error");
            close(fd);
            return 1;
        }
    }

    close(fd);
    return 0;
}

