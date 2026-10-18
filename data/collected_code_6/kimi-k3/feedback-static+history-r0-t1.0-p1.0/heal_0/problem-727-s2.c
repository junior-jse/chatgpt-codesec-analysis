#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int remove_non_alnum(const char *input, char *output, size_t output_size) {
    regex_t regex;
    regmatch_t match[1];
    const char *cursor = input;
    size_t out_idx = 0;
    int ret;

    if (input == NULL || output == NULL || output_size == 0) {
        return -1;
    }

    ret = regcomp(&regex, "[a-zA-Z0-9]", REG_EXTENDED);
    if (ret != 0) {
        return -1;
    }

    while (*cursor != '\0') {
        ret = regexec(&regex, cursor, 1, match, 0);
        if (ret == REG_NOMATCH) {
            break;
        } else if (ret != 0) {
            regfree(&regex);
            return -1;
        }

        if (out_idx + 1 >= output_size) {
            regfree(&regex);
            return -1;
        }

        output[out_idx++] = cursor[match[0].rm_so];
        cursor += match[0].rm_eo;
    }

    output[out_idx] = '\0';
    regfree(&regex);
    return 0;
}

int main(void) {
    const char *input = "Hello, World! 123 @#$";
    char output[256];

    if (remove_non_alnum(input, output, sizeof(output)) == 0) {
        printf("Original: %s\n", input);
        printf("Filtered: %s\n", output);
    } else {
        fprintf(stderr, "Error processing string\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}