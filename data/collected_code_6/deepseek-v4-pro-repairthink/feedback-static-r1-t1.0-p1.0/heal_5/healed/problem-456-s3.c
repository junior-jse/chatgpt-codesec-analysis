#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 100
#define MAX_LENGTH 256
#define BUF_SIZE (MAX_LENGTH + 2)

void reverse_string(char *str, size_t len) {
    if (str == NULL || len == 0) {
        return;
    }
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

void reverse_strings(char **strings, const size_t *lengths, int count) {
    if (strings == NULL || lengths == NULL || count <= 0) {
        return;
    }
    for (int i = 0; i < count; i++) {
        if (strings[i] != NULL && lengths[i] > 0) {
            reverse_string(strings[i], lengths[i]);
        }
    }
}

int main(void) {
    char **strings = NULL;
    size_t *lengths = NULL;
    int count = 0;
    char *buffer = NULL;
    char input_line[32];

    printf("Enter number of strings (max %d): ", MAX_STRINGS);
    if (fgets(input_line, sizeof(input_line), stdin) == NULL ||
        sscanf(input_line, "%d", &count) != 1 ||
        count <= 0 || count > MAX_STRINGS) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    strings = malloc(count * sizeof(char *));
    lengths = malloc(count * sizeof(size_t));
    if (strings == NULL || lengths == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(strings);
        free(lengths);
        return EXIT_FAILURE;
    }

    buffer = malloc(BUF_SIZE);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(strings);
        free(lengths);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < count; i++) {
        printf("Enter string %d: ", i + 1);
        if (fgets(buffer, BUF_SIZE, stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            for (int j = 0; j < i; j++) {
                free(strings[j]);
            }
            free(strings);
            free(lengths);
            free(buffer);
            return EXIT_FAILURE;
        }

        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        } else {
            char discard[2];
            while (fgets(discard, sizeof(discard), stdin) && !strchr(discard, '\n'));
        }

        strings[i] = malloc(len + 1);
        if (strings[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(strings[j]);
            }
            free(strings);
            free(lengths);
            free(buffer);
            return EXIT_FAILURE;
        }
        strncpy(strings[i], buffer, len);
        strings[i][len] = '\0';
        lengths[i] = len;
    }

    free(buffer);
    buffer = NULL;

    reverse_strings(strings, lengths, count);

    printf("\nReversed strings:\n");
    for (int i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
    }

    for (int i = 0; i < count; i++) {
        free(strings[i]);
    }
    free(strings);
    free(lengths);

    return EXIT_SUCCESS;
}