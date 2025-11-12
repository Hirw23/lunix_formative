#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

// Structure for passing arguments to each thread
typedef struct {
    char *input_file;
    int output_fd;
    off_t offset;
    pthread_mutex_t *lock;
} ThreadArgs;

// Function to get file size
off_t get_file_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0)
        return st.st_size;
    return -1;
}

// Thread function to read input and write to output
void* merge_file(void *arg) {
    ThreadArgs *data = (ThreadArgs *)arg;
    int in_fd = open(data->input_file, O_RDONLY);
    if (in_fd < 0) {
        perror("Error opening input file");
        pthread_exit(NULL);
    }

    char buffer[4096];
    ssize_t bytes_read;

    // Read from input and write to output at a specific offset
    off_t current_offset = data->offset;
    while ((bytes_read = read(in_fd, buffer, sizeof(buffer))) > 0) {
        pthread_mutex_lock(data->lock);  // Synchronize write
        lseek(data->output_fd, current_offset, SEEK_SET);
        write(data->output_fd, buffer, bytes_read);
        pthread_mutex_unlock(data->lock);
        current_offset += bytes_read;
    }

    close(in_fd);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s merged_log.txt file1.txt file2.txt ... <num_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[argc - 1]);
    int num_files = argc - 3; // subtract output file + num_threads
    if (num_threads > num_files)
        num_threads = num_files; // limit threads to available files

    int output_fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd < 0) {
        perror("Error creating output file");
        return 1;
    }

    pthread_t threads[num_threads];
    ThreadArgs args[num_threads];
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);

    // Calculate offsets for each input file
    off_t offset = 0;
    for (int i = 0; i < num_threads; i++) {
        args[i].input_file = argv[i + 2];
        args[i].output_fd = output_fd;
        args[i].offset = offset;
        args[i].lock = &lock;

        off_t size = get_file_size(args[i].input_file);
        if (size == -1) {
            perror("Error getting file size");
            close(output_fd);
            pthread_mutex_destroy(&lock);
            return 1;
        }
        offset += size;
    }

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, merge_file, &args[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    close(output_fd);

    printf("✅ Merging complete. Output saved to %s\n", argv[1]);
    return 0;
}

