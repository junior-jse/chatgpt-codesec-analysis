#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 4096

static size_t bounded_strlen(const char* s, size_t max_len) {
    size_t len = 0;
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

char* abbreviate_road(const char* input) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = bounded_strlen(input, MAX_INPUT_LENGTH);

    if (len == SIZE_MAX) {
        return NULL;
    }

    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    size_t j = 0;

    while (i < len) {
        if (i + 4 <= len &&
            tolower((unsigned char)input[i]) == 'r' &&
            tolower((unsigned char)input[i + 1]) == 'o' &&
            tolower((unsigned char)input[i + 2]) == 'a' &&
            tolower((unsigned char)input[i + 3]) == 'd') {

            result[j++] = 'r';
            result[j++] = 'd';
            result[j++] = '.';
            i += 4;
        } else {
            result[j++] = input[i++];
        }
    }

    result[j] = '\0';
    return result;
}

int main(void) {
    const char* test_strings[] = {
        "Take the road to success",
        "The Road less traveled",
        "ROAD work ahead",
        "Turn left at the next road",
        "road"
    };

    size_t num_tests = sizeof(test_strings) / sizeof(test_strings[0]);

    for (size_t i = 0; i < num_tests; i++) {
        char* abbreviated = abbreviate_road(test_strings[i]);
        if (abbreviated != NULL) {
            printf("Original: %s\n", test_strings[i]);
            printf("Abbreviated: %s\n\n", abbreviated);
            free(abbreviated);
        } else {
            fprintf(stderr, "Memory allocation failed\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}