#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void reverse_segment(char *start, char *end)
{
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

int reverse_words(char *str, size_t max_len)
{
    if (str == NULL || max_len == 0) {
        return -1;
    }

    size_t len = strnlen(str, max_len);
    if (len == 0) {
        return 0;
    }

    reverse_segment(str, str + len - 1);

    size_t word_start = 0;

    for (size_t i = 0; i < len; i++) {
        if (str[i] == ' ') {
            if (i > word_start) {
                reverse_segment(str + word_start, str + i - 1);
            }
            word_start = i + 1;
        }
    }
    if (len > word_start) {
        reverse_segment(str + word_start, str + len - 1);
    }

    return 0;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[256] = {0};

    printf("Enter a string: ");
    if (fgets(buffer, sizeof(buffer) - 1, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    buffer[sizeof(buffer) - 1] = '\0';

    size_t len = strnlen(buffer, sizeof(buffer));
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    if (reverse_words(buffer, sizeof(buffer)) != 0) {
        fprintf(stderr, "Error processing string\n");
        return EXIT_FAILURE;
    }

    printf("Reversed words: %s\n", buffer);

    return EXIT_SUCCESS;
}