#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **split_at_uppercase(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    int len = 0;
    int count = 1;
    while (input[len] != '\0') {
        if (isupper((unsigned char)input[len])) {
            count++;
        }
        len++;
    }

    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    int start = 0;
    int seg = 0;
    while (seg < count) {
        int end;
        if (seg == 0) {
            const char *p = input;
            while (*p && !isupper((unsigned char)*p)) p++;
            end = p - input;
        } else {
            const char *p = input + start + 1;
            while (*p && !isupper((unsigned char)*p)) p++;
            end = p - input;
        }
        if (seg == count - 1) {
            end = len;
        }
        int length = end - start;
        result[seg] = malloc(length + 1);
        if (result[seg] == NULL) {
            for (int j = 0; j < seg; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[seg], input + start, length);
        result[seg][length] = '\0';
        start = end;
        seg++;
    }
    result[count] = NULL;
    return result;
}

void free_split_result(char **result) {
    if (result == NULL) return;
    for (int i = 0; result[i] != NULL; i++) {
        free(result[i]);
    }
    free(result);
}

int main(void) {
    const char *test_string = "helloWorldThisIsATest";
    char **parts = split_at_uppercase(test_string);
    if (!parts) {
        fprintf(stderr, "Failed to split string\n");
        return 1;
    }
    printf("Original: %s\n", test_string);
    printf("Split parts:\n");
    for (int i = 0; parts[i] != NULL; i++) {
        printf("  [%d]: '%s'\n", i, parts[i]);
    }
    free_split_result(parts);
    return 0;
}