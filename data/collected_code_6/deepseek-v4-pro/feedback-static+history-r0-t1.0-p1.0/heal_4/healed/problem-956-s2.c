#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **split_at_uppercase(const char *input) {
    int count = 1;
    char **result;
    int i = 0;
    int start = 0;
    size_t len;
    size_t j;

    if (input == NULL) {
        return NULL;
    }

    len = strlen(input);
    if (len == 0) {
        result = malloc(2 * sizeof(char *));
        if (!result) {
            return NULL;
        }
        result[0] = malloc(1);
        if (!result[0]) {
            free(result);
            return NULL;
        }
        result[0][0] = '\0';
        result[1] = NULL;
        return result;
    }

    if (len > 4096) {
        return NULL;
    }

    for (j = 1; j < len; j++) {
        if (isupper((unsigned char)input[j])) {
            count++;
        }
    }

    result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    start = 0;
    i = 0;

    for (j = 1; j <= len; j++) {
        if (j == len || isupper((unsigned char)input[j])) {
            size_t part_len = (size_t)(j - start);
            result[i] = malloc(part_len + 1);
            if (!result[i]) {
                size_t k;
                for (k = 0; k < i; k++) {
                    free(result[k]);
                }
                free(result);
                return NULL;
            }
            memcpy(result[i], input + start, part_len);
            result[i][part_len] = '\0';
            start = j;
            i++;
        }
    }

    result[count] = NULL;
    return result;
}

void free_split_result(char **result) {
    int i = 0;
    if (!result) return;
    while (result[i] != NULL) {
        free(result[i]);
        i++;
    }
    free(result);
}

int main(void) {
    const char *test_string = "helloWorldThisIsATest";
    char **parts = split_at_uppercase(test_string);
    int i = 0;

    if (!parts) {
        fprintf(stderr, "Failed to split string\n");
        return 1;
    }

    printf("Original: %s\n", test_string);
    printf("Split parts:\n");
    while (parts[i] != NULL) {
        printf("  [%d]: '%s'\n", i, parts[i]);
        i++;
    }

    free_split_result(parts);
    return 0;
}