#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *abbreviate_road(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = strlen(input);
    char *result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    size_t j = 0;

    while (i < len) {
        if (i + 3 < len &&
            tolower((unsigned char)input[i]) == 'r' &&
            tolower((unsigned char)input[i + 1]) == 'o' &&
            tolower((unsigned char)input[i + 2]) == 'a' &&
            tolower((unsigned char)input[i + 3]) == 'd') {

            int word_start = (i == 0) || !isalnum((unsigned char)input[i - 1]);
            int word_end = (i + 4 >= len) || !isalnum((unsigned char)input[i + 4]);

            if (word_start && word_end) {
                result[j++] = 'r';
                result[j++] = 'd';
                result[j++] = '.';
                i += 4;
                continue;
            }
        }
        result[j++] = input[i++];
    }

    result[j] = '\0';
    return result;
}

int main(void) {
    const char *test_string = "Take the road to reach the main road. ROAD ahead! Broadway road";
    char *abbreviated = abbreviate_road(test_string);

    if (abbreviated != NULL) {
        printf("Original: %s\n", test_string);
        printf("Abbreviated: %s\n", abbreviated);
        free(abbreviated);
    } else {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}