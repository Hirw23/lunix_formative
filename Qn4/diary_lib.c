#include <stdio.h>
#include <string.h>

#define KEY 4  // Symmetric key

void encrypt(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        text[i] = text[i] + KEY;  // simple Caesar cipher
    }
}

void decrypt(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        text[i] = text[i] - KEY;
    }
}


