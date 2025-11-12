#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *fp = fopen("buffered_log.txt", "w");
    if (fp == NULL) {
        perror("File open failed");
        return 1;
    }

    time_t now;

    for (int i = 0; i < 1000000; i++) {
        now = time(NULL);
        fprintf(fp, "Entry %d: %s", i + 1, ctime(&now));
    }

    fclose(fp);
    return 0;
}

