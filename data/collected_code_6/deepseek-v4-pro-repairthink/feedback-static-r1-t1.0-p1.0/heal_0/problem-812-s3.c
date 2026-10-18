#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *abbreviate_road(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t input_len = strlen(input);
    char *result = malloc(input_len * 2 + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
    while (i < input_len) {
        if (i + 3 < input_len &&
            tolower(input[i]) == 'r' &&
            tolower(input[i + 1]) == 'o' &&
            tolower(input[i + 2]) == 'a' &&
            tolower(input[i + 3]) == 'd') {
            
            if ((i == 0 || !isalpha(input[i - 1])) &&
                (i + 4 >= input_len || !isalpha(input[i + 4]))) {
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
    const char *test1 = "123 road street";
    const char *test2 = "road road road";
    const char *test3 = "main street road";
    const char *test4 = "roads and driveways";
    const char *test5 = "Road";
    const char *test6 = NULL;

    char *results[6];
    results[0] = abbreviate_road(test1);
    results[1] = abbreviate_road(test2);
    results[2] = abbreviate_road(test3);
    results[3] = abbreviate_road(test4);
    results[4] = abbreviate_road(test5);
    results[5] = abbreviate_road(test6);

    for (int i = 0; i < 6; i++) {
        if (results[i] != NULL) {
            printf("%s\n", results[i]);
            free(results[i]);
        } else {
            printf("(null or allocation failed)\n");
        }
    }

    return 0;
}