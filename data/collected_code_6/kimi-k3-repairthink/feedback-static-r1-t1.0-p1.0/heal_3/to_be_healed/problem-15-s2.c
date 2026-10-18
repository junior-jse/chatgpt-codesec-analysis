#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 4096

char **split_at_lowercase(const char *str, size_t max_len, size_t *count) {
    if (str == NULL || count == NULL || max_len == 0) {
        return NULL;
    }

    *count = 0;

    size_t len = strnlen(str, max_len);
    if (len == 0) {
        return NULL;
    }

    char **result = malloc((len + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    size_t start = 0;
    size_t idx = 0;

    for (size_t i = 0; i <= len; i++) {
        if (i == len || islower((unsigned char)str[i])) {
            size_t segment_len = i - start;
            if (segment_len > 0) {
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
                /* Possible weaknesses found:
                 *  Condition 'segment_len<buf_size' is always true [knownConditionTrueFalse]
                 *  Condition 'segment_len<buf_size' is always true
                 */
                if (segment_len < buf_size && start + segment_len <= len) {
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(result[idx], str + start, segment_len);
                    result[idx][segment_len] = '\0';
                    idx++;
                } else {
                    free(result[idx]);
                    for (size_t j = 0; j < idx; j++) {
                        free(result[j]);
                    }
                    free(result);
                    return NULL;
                }
            }
            start = i + 1;
        }
    }

    *count = idx;
    return result;
}

void free_split_result(char **result, size_t count) {
    if (result == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(result[i]);
    }
    free(result);
}

int main(void) {
    const char *input = "HelloWorldThisIsATest";
    size_t count = 0;

    char **segments = split_at_lowercase(input, MAX_INPUT_LENGTH, &count);

    if (segments == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Input: %s\n", input);
    printf("Segments found: %zu\n", count);

    for (size_t i = 0; i < count; i++) {
        printf("Segment %zu: %s\n", i + 1, segments[i]);
    }

    free_split_result(segments, count);

    return EXIT_SUCCESS;
}