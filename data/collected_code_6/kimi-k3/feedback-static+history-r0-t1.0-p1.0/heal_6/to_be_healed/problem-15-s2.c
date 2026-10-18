/* Repair history (previous round):
 * - Flawfinder strncpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **split_at_lowercase(const char *str, size_t max_len, int *count) {
    if (str == NULL || count == NULL) {
        return NULL;
    }
    
    size_t len = strnlen(str, max_len);
    if (len == 0) {
        *count = 0;
        return NULL;
    }
    
    char **result = calloc(len + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    size_t start = 0;
    int idx = 0;
    
    for (size_t i = 0; i <= len; i++) {
        if (str[i] == '\0' || islower((unsigned char)str[i])) {
            size_t segment_len = i - start;
            if (segment_len > 0) {
                result[idx] = calloc(segment_len + 1, sizeof(char));
                if (result[idx] == NULL) {
                    for (int j = 0; j < idx; j++) {
                        free(result[j]);
                    }
                    free(result);
                    return NULL;
                }
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result[idx], str + start, segment_len);
                result[idx][segment_len] = '\0';
                idx++;
            }
            start = i + 1;
        }
    }
    
    *count = idx;
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
    size_t max_input_len = 1024;
    int count = 0;
    
    char **segments = split_at_lowercase(input, max_input_len, &count);
    
    if (segments == NULL && count > 0) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Input: %s\n", input);
    printf("Segments found: %d\n", count);
    
    for (int i = 0; i < count; i++) {
        printf("Segment %d: %s\n", i + 1, segments[i]);
    }
    
    free_split_result(segments, count);
    
    return EXIT_SUCCESS;
}