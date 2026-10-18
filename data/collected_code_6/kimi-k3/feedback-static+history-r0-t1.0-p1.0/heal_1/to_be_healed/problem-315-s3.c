#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int find_first_max_even_word_length(const char *str) {
    int max_len = 0;
    int curr_len = 0;
    int in_word = 0;
    const char *p = str;

    if (str == NULL) {
        return 0;
    }

    while (*p != '\0') {
        if (isspace((unsigned char)*p)) {
            if (in_word) {
                if (curr_len % 2 == 0 && curr_len > max_len) {
                    max_len = curr_len;
                }
                in_word = 0;
                curr_len = 0;
            }
        } else {
            in_word = 1;
            curr_len++;
        }
        p++;
    }

    if (in_word && curr_len % 2 == 0 && curr_len > max_len) {
        max_len = curr_len;
    }

    return max_len;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[1024];

    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    int result = find_first_max_even_word_length(input);
    printf("Maximum length of even word: %d\n", result);

    return EXIT_SUCCESS;
}