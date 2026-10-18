#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **split_at_uppercase(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t count = 1;
    const char *p = input;
    while (*p != '\0') {
        if (isupper((unsigned char)*p)) {
            count++;
        }
        p++;
    }
    size_t len = p - input;

    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    size_t seg = 0;
    while (seg < count) {
        size_t start = i;
        while (i < len && !isupper((unsigned char)input[i])) {
            i++;
        }
        size_t length = i - start;
        result[seg] = malloc(length + 1);
        if (result[seg] == NULL) {
            for (size_t j = 0; j < seg; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        snprintf(result[seg], length + 1, "%.*s", (int)length, input + start);
        if (i < len && isupper((unsigned char)input[i])) {
            i++;
        }
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