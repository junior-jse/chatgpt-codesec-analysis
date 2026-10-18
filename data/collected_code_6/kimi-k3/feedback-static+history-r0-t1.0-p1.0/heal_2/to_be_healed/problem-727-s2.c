#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int remove_non_alnum(const char *input, char *output, size_t output_size) {
    size_t out_idx = 0;

    if (input == NULL || output == NULL || output_size == 0) {
        return -1;
    }

    while (*input != '\0') {
        if (isalnum((unsigned char)*input)) {
            if (out_idx + 1 >= output_size) {
                return -1;
            }
            output[out_idx++] = *input;
        }
        input++;
    }

    output[out_idx] = '\0';
    return 0;
}

int main(void) {
    const char *input = "Hello, World! 123 @#$";
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
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