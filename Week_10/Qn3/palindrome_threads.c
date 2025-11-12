#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_NUM 100000
#define THREAD_COUNT 20

int total_palindromes = 0;
pthread_mutex_t lock;

// Function to check if a number is a palindrome
bool is_palindrome(int n) {
    int original = n, reversed = 0, digit;
    while (n > 0) {
        digit = n % 10;
        reversed = reversed * 10 + digit;
        n /= 10;
    }
    return original == reversed;
}

// Thread function
void* count_palindromes(void* arg) {
    int thread_id = *(int*)arg;
    int segment_size = MAX_NUM / THREAD_COUNT;
    int start = thread_id * segment_size;
    int end = (thread_id == THREAD_COUNT - 1) ? MAX_NUM : start + segment_size - 1;

    int local_count = 0;
    for (int i = start; i <= end; i++) {
        if (is_palindrome(i)) {
            local_count++;
        }
    }

    // Synchronize access to global counter
    pthread_mutex_lock(&lock);
    total_palindromes += local_count;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int thread_ids[THREAD_COUNT];

    // Initialize mutex
    pthread_mutex_init(&lock, NULL);

    // Create threads
    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, count_palindromes, &thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy mutex
    pthread_mutex_destroy(&lock);

    printf("The synchronized total number of palindromic numbers from 0 to %d is %d\n", MAX_NUM, total_palindromes);

    return 0;
}

