#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **extract_strings(char **strings, size_t count, size_t size, size_t *out_count) {
    /* Possible weaknesses found:
     *  Assuming that condition 'out_count==NULL' is not redundant
     */
    if (strings == NULL || out_count == NULL || count == 0 || size == 0) {
        /* Possible weaknesses found:
         *  Null pointer dereference
         *  Either the condition 'out_count==NULL' is redundant or there is possible null pointer dereference: out_count. [nullPointerRedundantCheck]
         */
        *out_count = 0;
        return NULL;
    }

    char **result = (char **)malloc(count * sizeof(char *));
    if (result == NULL) {
        *out_count = 0;
        return NULL;
    }

    size_t valid_count = 0;
    for (size_t i = 0; i < count; i++) {
        if (strings[i] == NULL) {
            continue;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        if (strlen(strings[i]) < size) {
            continue;
        }

        result[valid_count] = (char *)malloc((size + 1) * sizeof(char));
        if (result[valid_count] == NULL) {
            for (size_t j = 0; j < valid_count; j++) {
                free(result[j]);
            }
            free(result);
            *out_count = 0;
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
         */
        strncpy(result[valid_count], strings[i], size);
        result[valid_count][size] = '\0';
        valid_count++;
    }

    if (valid_count == 0) {
        free(result);
        *out_count = 0;
        return NULL;
    }

    *out_count = valid_count;
    return result;
}

int main(void) {
    char *strings[] = {
        "hello",
        "world",
        "programming",
        "in",
        "c",
        "language"
    };
    size_t count = sizeof(strings) / sizeof(strings[0]);
    size_t extract_size = 3;
    size_t result_count = 0;

    char **extracted = extract_strings(strings, count, extract_size, &result_count);

    if (extracted != NULL) {
        for (size_t i = 0; i < result_count; i++) {
            printf("%s\n", extracted[i]);
            free(extracted[i]);
        }
        free(extracted);
    } else {
        printf("No strings extracted or error occurred.\n");
    }

    return 0;
}