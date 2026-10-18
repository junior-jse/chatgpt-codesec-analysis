#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *abbreviate_road(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = strlen(input);
    char *result = malloc(len * 2 + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
    while (i < len) {
        if (strncasecmp(input + i, "road", 4) == 0) {
            size_t end = i + 4;
            int word_boundary_start = (i == 0) || !isalnum((unsigned char)input[i - 1]);
            int word_boundary_end = (end >= len) || !isalnum((unsigned char)input[end]);
            
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
        char *abbreviated = abbreviate_road(test_strings[i]);
        if (abbreviated != NULL) {
            printf("Original: %s\n", test_strings[i]);
            printf("Abbreviated: %s\n\n", abbreviated);
            free(abbreviated);
        }
    }

    return 0;
}