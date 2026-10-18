#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *abbreviate_road(const char *input, size_t input_len) {
    if (input == NULL) {
        return NULL;
    }

    char *result = malloc(input_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
    while (i < input_len) {
        if (i + 4 <= input_len && strncasecmp(input + i, "road", 4) == 0) {
            size_t end = i + 4;
            int word_boundary_start = (i == 0) || !isalnum((unsigned char)input[i - 1]);
            int word_boundary_end = (end >= input_len) || !isalnum((unsigned char)input[end]);
            
            if (word_boundary_start && word_boundary_end) {
                result[j++] = 'r';
                result[j++] = 'd';
                result[j++] = '.';
                i = end;
                continue;
            }
        }
        result[j++] = input[i++];
    }
    result[j] = '\0';
    return result;
}

int main(void) {
    const char *test_strings[] = {
        "Main road to the park",
        "Road trip on the highway",
        "My address is 123 Broad road",
        "The roadrunner runs on the road",
        NULL
    };

    for (int i = 0; test_strings[i] != NULL; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len = strlen(test_strings[i]);
        char *abbreviated = abbreviate_road(test_strings[i], len);
        if (abbreviated != NULL) {
            printf("Original: %s\n", test_strings[i]);
            printf("Abbreviated: %s\n\n", abbreviated);
            free(abbreviated);
        }
    }

    return 0;
}