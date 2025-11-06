#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 100
#define ENTRY_SIZE 256

// Declare shared library functions manually
void encrypt(char *text);
void decrypt(char *text);

// In-memory storage
char diary[MAX_ENTRIES][ENTRY_SIZE];
int entry_count = 0;

// Simple password verification
int verify_user() {
    char password[20];
    printf("Enter password to access diary: ");
    scanf("%s", password);
    if (strcmp(password, "1234") == 0) {  // simple password
        return 1;
    }
    printf("Verification failed!\n");
    return 0;
}

// Add diary entry (encrypt before storing)
void add_entry() {
    if (entry_count >= MAX_ENTRIES) {
        printf("Diary full!\n");
        return;
    }

    char entry[ENTRY_SIZE];
    printf("Write your diary entry: ");
    getchar();  // consume leftover newline
    fgets(entry, ENTRY_SIZE, stdin);
    entry[strcspn(entry, "\n")] = '\0';  // remove newline

    encrypt(entry);
    strcpy(diary[entry_count], entry);
    entry_count++;
    printf("Entry added and encrypted!\n");
}

// View all entries (decrypt before showing)
void view_entries() {
    if (!verify_user()) return;

    printf("\n--- Your Diary Entries ---\n");
    for (int i = 0; i < entry_count; i++) {
        char temp[ENTRY_SIZE];
        strcpy(temp, diary[i]);
        decrypt(temp);
        printf("%d: %s\n", i + 1, temp);
    }
    printf("--------------------------\n");
}

int main() {
    int choice;
    while (1) {
        printf("\nDiary Manager\n");
        printf("1. Add Entry\n");
        printf("2. View Entries\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_entry();
                break;
            case 2:
                view_entries();
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}

