#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_INPUT 1024

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
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[MAX_INPUT];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char output[MAX_INPUT];

    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    input[strcspn(input, "\n")] = '\0';

    remove_non_alnum(input, output, sizeof(output));

    printf("Result: %s\n", output);

    return EXIT_SUCCESS;
}