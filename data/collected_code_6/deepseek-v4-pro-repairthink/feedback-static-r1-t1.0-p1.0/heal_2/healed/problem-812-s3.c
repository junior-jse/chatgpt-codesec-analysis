#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SAFE_STRING 4096

char *abbreviate_road(const char *input, size_t max_len) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = 0;
    while (len < max_len && input[len] != '\0') {
        ++len;
    }

    char *result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
    while (i < len) {
        if (i + 3 < len &&
            tolower((unsigned char)input[i]) == 'r' &&
            tolower((unsigned char)input[i + 1]) == 'o' &&
            tolower((unsigned char)input[i + 2]) == 'a' &&
            tolower((unsigned char)input[i + 3]) == 'd') {

            int word_start = (i == 0) || !isalpha((unsigned char)input[i - 1]);
            int word_end = (i + 4 >= len) || !isalpha((unsigned char)input[i + 4]);

            if (word_start && word_end) {
                result[j++] = input[i];
                result[j++] = 'd';
                result[j++] = '.';
                i += 4;
                continue;
            }
        }
        result[j++] = input[i++];
    }
    result[j] = '\0';

    char *shrunk = realloc(result, j + 1);
    if (shrunk != NULL) {
        result = shrunk;
    }

    return result;
}

int main(void) {
    const char *tests[] = {
        "123 road street",
        "road road road",
        "main street road",
        "roads and driveways",
        "Road",
        NULL
    };
    size_t num_tests = sizeof(tests) / sizeof(tests[0]);

    for (size_t i = 0; i < num_tests; ++i) {
        const char *input = tests[i];
        size_t max_len = 0;
        if (input != NULL) {
            while (max_len < MAX_SAFE_STRING && input[max_len] != '\0') {
                ++max_len;
            }
        }
        char *abbreviated = abbreviate_road(input, max_len);
        if (abbreviated != NULL) {
            printf("%s\n", abbreviated);
            free(abbreviated);
        } else {
            printf("(null or allocation failed)\n");
        }
    }

    return 0;
}