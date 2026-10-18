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

int reverse_words(char *str, size_t size)
{
    if (str == NULL || size == 0) {
        return -1;
    }

    size_t len = strnlen(str, size);
    if (len == size) {
        return -1;
    }
    if (len == 0) {
        return 0;
    }

    reverse_segment(str, str + len - 1);

    char *word_start = str;
    char *current = str;

    while (*current != '\0') {
        if (*current == ' ') {
            reverse_segment(word_start, current - 1);
            word_start = current + 1;
        }
        current++;
    }
    reverse_segment(word_start, current - 1);

    return 0;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[256];

    printf("Enter a string: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    size_t len = strnlen(buffer, sizeof(buffer));
    if (len == sizeof(buffer)) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
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