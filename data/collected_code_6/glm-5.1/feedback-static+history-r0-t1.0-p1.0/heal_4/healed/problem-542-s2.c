#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* replace_with_colon(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }

    char* result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < len; ++i) {
        char c = str[i];
        if (c == ' ' || c == ',' || c == '.') {
            result[i] = ':';
        } else {
            result[i] = c;
        }
    }
    result[len] = '\0';

    return result;
}

int main(int argc, char* const argv[const]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    char* modified = replace_with_colon(argv[1]);
    if (modified == NULL) {
        fprintf(stderr, "Failed to process string\n");
        return 1;
    }

    printf("%s\n", modified);
    free(modified);

    return 0;
}