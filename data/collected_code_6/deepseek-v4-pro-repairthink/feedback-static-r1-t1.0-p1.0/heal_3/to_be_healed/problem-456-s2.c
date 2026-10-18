#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str) {
    if (str == NULL) return;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t length = strlen(str);
    for (size_t i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - 1 - i];
        str[length - 1 - i] = temp;
    }
}

void reverse_strings(char **list, size_t count) {
    if (list == NULL || count == 0) return;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            reverse_string(list[i]);
        }
    }
}

int main(void) {
    const char *initial[] = {
        "hello",
        "world",
        "example",
        "programming"
    };
    size_t num_strings = sizeof(initial) / sizeof(initial[0]);

    char **list = malloc(num_strings * sizeof(char *));
    if (list == NULL) return 1;

    for (size_t i = 0; i < num_strings; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len = strlen(initial[i]);
        list[i] = malloc(len + 1);
        if (list[i] == NULL) {
            for (size_t j = 0; j < i; j++) free(list[j]);
            free(list);
            return 1;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(list[i], initial[i], len + 1);
    }

    reverse_strings(list, num_strings);

    for (size_t i = 0; i < num_strings; i++) {
        printf("%s\n", list[i]);
        free(list[i]);
    }
    free(list);

    return 0;
}