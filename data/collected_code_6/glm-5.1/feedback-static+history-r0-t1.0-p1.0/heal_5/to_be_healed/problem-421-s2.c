#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* concatenate_tuple(const char* elements[], size_t count, const char* delimiter) {
    if (count == 0 || elements == NULL || delimiter == NULL) {
        char* result = malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    size_t total_length = 0;
    size_t delimiter_length = strnlen(delimiter, SIZE_MAX);

    for (size_t i = 0; i < count; i++) {
        if (elements[i] != NULL) {
            size_t elem_len = strnlen(elements[i], SIZE_MAX);
            if (total_length > SIZE_MAX - elem_len) return NULL;
            total_length += elem_len;
            if (i < count - 1) {
                if (total_length > SIZE_MAX - delimiter_length) return NULL;
                total_length += delimiter_length;
            }
        }
    }

    if (total_length == SIZE_MAX) return NULL;

    char* result = malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t current_pos = 0;

    for (size_t i = 0; i < count; i++) {
        if (elements[i] != NULL) {
            size_t elem_len = strnlen(elements[i], total_length - current_pos + 1);
            if (elem_len > 0) {
                size_t remaining = total_length - current_pos;
                size_t to_copy = elem_len < remaining ? elem_len : remaining;
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + current_pos, elements[i], to_copy);
            }
            current_pos += elem_len;
            
            if (i < count - 1) {
                if (delimiter_length > 0) {
                    size_t remaining = total_length - current_pos;
                    size_t to_copy = delimiter_length < remaining ? delimiter_length : remaining;
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(result + current_pos, delimiter, to_copy);
                }
                current_pos += delimiter_length;
            }
        }
    }

    result[current_pos] = '\0';

    return result;
}

int main(void) {
    const char* tuple[] = {"apple", "banana", "cherry"};
    size_t tuple_size = sizeof(tuple) / sizeof(tuple[0]);
    const char* delimiter = ", ";

    char* concatenated = concatenate_tuple(tuple, tuple_size, delimiter);
    
    if (concatenated != NULL) {
        printf("%s\n", concatenated);
        free(concatenated);
    }

    return 0;
}