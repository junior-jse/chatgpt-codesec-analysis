#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

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
    char *buffer = NULL;
    size_t buffer_size = 0;
    /* Possible weaknesses found:
     * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    ssize_t read;

    printf("Enter a string: ");
    read = getline(&buffer, &buffer_size, stdin);
    /* Possible weaknesses found:
     * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    if (read < 0) {
        fprintf(stderr, "Error reading input\n");
        free(buffer);
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    if (read > 0 && buffer[read - 1] == '\n') {
        buffer[read - 1] = '\0';
    }

    if (reverse_words(buffer, buffer_size) != 0) {
        fprintf(stderr, "Error processing string\n");
        free(buffer);
        return EXIT_FAILURE;
    }

    printf("Reversed words: %s\n", buffer);

    free(buffer);
    return EXIT_SUCCESS;
}