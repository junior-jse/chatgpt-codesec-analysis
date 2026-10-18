#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MAX_INPUT_LENGTH 4096

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
    ssize_t nread;

    printf("Enter a string: ");

    nread = getline(&buffer, &buffer_size, stdin);
    if (nread < 0) {
        fprintf(stderr, "Error reading input\n");
        free(buffer);
        return EXIT_FAILURE;
    }

    if ((size_t)nread > MAX_INPUT_LENGTH) {
        fprintf(stderr, "Input exceeds maximum allowed length\n");
        free(buffer);
        return EXIT_FAILURE;
    }

    if (buffer_size == 0 || strnlen(buffer, buffer_size) >= buffer_size) {
        fprintf(stderr, "Input buffer is not properly terminated\n");
        free(buffer);
        return EXIT_FAILURE;
    }

    if (nread > 0 && buffer[nread - 1] == '\n') {
        buffer[nread - 1] = '\0';
        nread--;
    }

    if (reverse_words(buffer, (size_t)nread + 1) != 0) {
        fprintf(stderr, "Error processing string\n");
        free(buffer);
        return EXIT_FAILURE;
    }

    printf("Reversed words: %s\n", buffer);

    free(buffer);
    return EXIT_SUCCESS;
}