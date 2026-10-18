#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int remove_words_with_length(char *str, size_t k)
{
    size_t read_pos = 0;
    size_t write_pos = 0;

    if (str == NULL) {
        return -1;
    }

    while (str[read_pos] != '\0') {
        size_t separator_start = read_pos;
        size_t separator_length;
        size_t word_start;
        size_t word_length;

        while (str[read_pos] != '\0' &&
               isspace((unsigned char)str[read_pos]) != 0) {
            ++read_pos;
        }

        separator_length = read_pos - separator_start;
        word_start = read_pos;

        while (str[read_pos] != '\0' &&
               isspace((unsigned char)str[read_pos]) == 0) {
            ++read_pos;
        }

        word_length = read_pos - word_start;

        if (word_length == 0) {
            memmove(str + write_pos, str + separator_start, separator_length);
            write_pos += separator_length;
        } else if (word_length != k) {
            memmove(str + write_pos, str + separator_start, separator_length);
            write_pos += separator_length;
            memmove(str + write_pos, str + word_start, word_length);
            write_pos += word_length;
        }
    }

    str[write_pos] = '\0';
    return 0;
}

int main(void)
{
    char input[4096];
    char length_input[128];
    char *end = NULL;
    unsigned long long parsed_length;
    size_t k;

    if (fgets(input, sizeof input, stdin) == NULL) {
        return EXIT_FAILURE;
    }

    if (strchr(input, '\n') == NULL && !feof(stdin)) {
        return EXIT_FAILURE;
    }

    input[strcspn(input, "\n")] = '\0';

    if (fgets(length_input, sizeof length_input, stdin) == NULL) {
        return EXIT_FAILURE;
    }

    errno = 0;
    parsed_length = strtoull(length_input, &end, 10);

    if (errno != 0 || end == length_input) {
        return EXIT_FAILURE;
    }

    while (isspace((unsigned char)*end) != 0) {
        ++end;
    }

    if (*end != '\0' || parsed_length > SIZE_MAX) {
        return EXIT_FAILURE;
    }

    k = (size_t)parsed_length;

    if (remove_words_with_length(input, k) != 0) {
        return EXIT_FAILURE;
    }

    if (puts(input) == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}