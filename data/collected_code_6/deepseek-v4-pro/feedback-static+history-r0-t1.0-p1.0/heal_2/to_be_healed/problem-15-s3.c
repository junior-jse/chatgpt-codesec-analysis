/* Repair history (previous round):
 * - Flawfinder strncpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** split_at_lowercase(const char* str, int* count) {
    if (str == NULL || count == NULL) {
        return NULL;
    }

    int len = 0;
    /* Possible weaknesses found:
     *  Either the condition 'len>1000000' is redundant or the array 'str[22]' is accessed at index 1000000, which is out of bounds. [arrayIndexOutOfBoundsCond]
     *  Array index out of bounds
     */
    while (str[len] != '\0') {
        len++;
        /* Possible weaknesses found:
         *  Assuming that condition 'len>1000000' is not redundant
         */
        if (len > 1000000) {
            return NULL;
        }
    }

    int capacity = 10;
    char** result = malloc(capacity * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    int segment_count = 0;
    int segment_start = 0;
    int i;

    for (i = 0; i <= len; i++) {
        if (i == len || islower((unsigned char)str[i])) {
            int segment_len = i - segment_start;
            if (segment_len > 0) {
                if (segment_count >= capacity) {
                    capacity *= 2;
                    char** temp = realloc(result, capacity * sizeof(char*));
                    if (temp == NULL) {
                        for (int j = 0; j < segment_count; j++) {
                            free(result[j]);
                        }
                        free(result);
                        return NULL;
                    }
                    result = temp;
                }

                result[segment_count] = malloc((segment_len + 1) * sizeof(char));
                if (result[segment_count] == NULL) {
                    for (int j = 0; j < segment_count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    return NULL;
                }

                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result[segment_count], str + segment_start, segment_len);
                result[segment_count][segment_len] = '\0';
                segment_count++;
            }
            segment_start = i + 1;
        }
    }

    if (segment_count == 0) {
        free(result);
        *count = 0;
        return NULL;
    }

    *count = segment_count;
    return result;
}

int main() {
    const char* test_string = "HelloWorldThisIsATest";
    int count = 0;
    char** parts = split_at_lowercase(test_string, &count);

    if (parts == NULL) {
        printf("No parts found or error occurred\n");
        return 0;
    }

    printf("Split results:\n");
    for (int i = 0; i < count; i++) {
        printf("%d: %s\n", i, parts[i]);
        free(parts[i]);
    }
    free(parts);

    return 0;
}