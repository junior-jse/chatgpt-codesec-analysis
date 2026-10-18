#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/types.h>

void remove_non_alnum(const char *input, char *output, size_t output_size) {
    regex_t regex;
    regmatch_t match;
    const char *cursor;
    size_t out_idx = 0;
    int ret;

    if (input == NULL || output == NULL || output_size == 0) {
        return;
    }

    ret = regcomp(&regex, "[A-Za-z0-9]", REG_EXTENDED);
    if (ret != 0) {
        output[0] = '\0';
        return;
    }

    cursor = input;
    while (*cursor != '\0' && out_idx < output_size - 1) {
        ret = regexec(&regex, cursor, 1, &match, 0);
        if (ret == 0 && match.rm_so == 0) {
            output[out_idx++] = cursor[0];
        }
        cursor++;
    }
    output[out_idx] = '\0';

    regfree(&regex);
}

int main(void) {
    char *input = NULL;
    char *output = NULL;
    size_t input_capacity = 0;
    ssize_t bytes_read;
    size_t input_length;

    printf("Enter a string: ");
    bytes_read = getline(&input, &input_capacity, stdin);
    if (bytes_read == -1) {
        fprintf(stderr, "Error reading input\n");
        free(input);
        return EXIT_FAILURE;
    }

    input[strcspn(input, "\n")] = '\0';
    input_length = strlen(input);

    output = malloc(input_length + 1);
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(input);
        return EXIT_FAILURE;
    }

    remove_non_alnum(input, output, input_length + 1);

    printf("Result: %s\n", output);

    free(input);
    free(output);

    return EXIT_SUCCESS;
}