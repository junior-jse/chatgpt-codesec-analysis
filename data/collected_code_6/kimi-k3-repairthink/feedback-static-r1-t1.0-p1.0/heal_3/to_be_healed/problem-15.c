#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 4096

char **split_at_lowercase(const char *str, int *count) {
    if (str == NULL || count == NULL) {
        return NULL;
    }

    *count = 0;

    size_t len = strnlen(str, MAX_INPUT_LENGTH);
    if (len == 0) {
        return NULL;
    }

    size_t max_splits = 1;
    for (size_t i = 0; i < len; i++) {
        if (islower((unsigned char)str[i])) {
            max_splits++;
        }
    }

    if (max_splits > SIZE_MAX / sizeof(char *)) {
        return NULL;
    }

    char **result = malloc(max_splits * sizeof(*result));
    if (result == NULL) {
        return NULL;
    }

    size_t start = 0;
    size_t idx = 0;

    for (size_t i = 0; i <= len; i++) {
        if (i == len || islower((unsigned char)str[i])) {
            size_t segment_len = i - start;
            /* Possible weaknesses found:
             *  buf_size is assigned 'segment_len+1' here.
             */
            size_t buf_size = segment_len + 1;

            result[idx] = malloc(buf_size);
            /* Possible weaknesses found:
             *  Assuming condition is false
             */
            if (result[idx] == NULL) {
                for (size_t j = 0; j < idx; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }

            if (segment_len > 0) {
                /* Possible weaknesses found:
                 *  Condition 'segment_len+1>buf_size' is always false [knownConditionTrueFalse]
                 *  Condition 'segment_len+1>buf_size' is always false
                 */
                if (segment_len + 1 > buf_size) {
                    for (size_t j = 0; j <= idx; j++) {
                        free(result[j]);
                    }
                    free(result);
                    return NULL;
                }
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result[idx], str + start, segment_len);
            }
            result[idx][segment_len] = '\0';
            idx++;
            start = i;
        }
    }

    *count = (int)idx;
    return result;
}

void free_split_result(char **result, int count) {
    if (result == NULL) {
        return;
    }
    for (int i = 0; i < count; i++) {
        free(result[i]);
    }
    free(result);
}

int main(void) {
    const char *input = "HelloWorldThisIsATest";
    int count = 0;

    char **segments = split_at_lowercase(input, &count);
    if (segments == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Input: %s\n", input);
    printf("Segments (%d):\n", count);
    for (int i = 0; i < count; i++) {
        printf("[%d]: \"%s\"\n", i, segments[i]);
    }

    free_split_result(segments, count);

    return EXIT_SUCCESS;
}