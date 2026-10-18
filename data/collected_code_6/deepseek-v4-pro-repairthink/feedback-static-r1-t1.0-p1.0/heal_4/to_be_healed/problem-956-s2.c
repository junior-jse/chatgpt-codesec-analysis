#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **split_at_uppercase(const char *input) {
    if (input == NULL) {
        return NULL;
    }
    int count = 1;
    for (const char *p = input; *p != '\0'; p++) {
        if (isupper((unsigned char)*p)) {
            count++;
        }
    }
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    int len = (int)strlen(input);
    int i = 0;
    int seg = 0;
    while (seg < count) {
        int start = i;
        while (i < len && !isupper((unsigned char)input[i])) {
            i++;
        }
        int length = i - start;
        result[seg] = malloc(length + 1);
        if (result[seg] == NULL) {
            for (int j = 0; j < seg; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        snprintf(result[seg], length + 1, "%.*s", length, input + start);
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