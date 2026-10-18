/* Repair history (previous round):
 * - Flawfinder strlen was removed from replace_with_colon() and now appears in main().
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* replace_with_colon(const char* str, size_t str_len) {
    if (str == NULL) {
        return NULL;
    }

    if (str_len == 0) {
        char* empty = malloc(1);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }

    size_t bufsize = str_len + 1;
    char* result = malloc(bufsize);
    if (result == NULL) {
        return NULL;
    }

    const char* p = str;
    size_t offset = 0;

    while ((size_t)(p - str) < str_len) {
        if (*p == ' ' || *p == ',' || *p == '.') {
            if (offset + 1 >= bufsize) {
                bufsize = (offset + 2) * 2;
                char* new_result = realloc(result, bufsize);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[offset++] = ':';
            p++;
        } else {
            if (offset + 1 >= bufsize) {
                bufsize = (offset + 2) * 2;
                char* new_result = realloc(result, bufsize);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[offset++] = *p;
            p++;
        }
    }

    result[offset] = '\0';

    return result;
}

int main(int argc, char* argv[]) {
    const char* input = "Hello world, this.is a test";
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(input);

    char* output = replace_with_colon(input, len);
    if (output == NULL) {
        fprintf(stderr, "Regex replacement failed\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", output);

    free(output);
    return EXIT_SUCCESS;
}