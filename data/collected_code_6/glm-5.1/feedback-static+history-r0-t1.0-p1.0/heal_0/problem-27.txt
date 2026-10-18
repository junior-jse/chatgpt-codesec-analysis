#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *remove_digits(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (!isdigit((unsigned char)str[i])) {
            result[j++] = str[i];
        }
    }

    result[j] = '\0';

    char *shrunk = realloc(result, j + 1);
    if (shrunk != NULL) {
        result = shrunk;
    }

    return result;
}

void free_string_list(char **list, size_t size) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < size; i++) {
        free(list[i]);
    }
    free(list);
}

int main(void) {
    const char *input[] = {
        "C0ding is 4w3s0m3!",
        "12345",
        "No digits here",
        "4ppl3 42"
    };
    size_t count = sizeof(input) / sizeof(input[0]);

    char **result_list = malloc(count * sizeof(char *));
    if (result_list == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; i++) {
        result_list[i] = remove_digits(input[i]);
        if (result_list[i] == NULL) {
            free_string_list(result_list, i);
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < count; i++) {
        printf("%s\n", result_list[i]);
    }

    free_string_list(result_list, count);

    return EXIT_SUCCESS;
}