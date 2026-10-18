#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int remove_words_of_length(char *string, size_t k)
{
    char *read_ptr;
    char *write_ptr;
    int pending_space = 0;
    int written_word = 0;

    if (string == NULL) {
        return -1;
    }

    read_ptr = string;
    write_ptr = string;

    while (*read_ptr != '\0') {
        char *word_start;
        size_t word_length;

        while (*read_ptr != '\0' &&
               isspace((unsigned char)*read_ptr) != 0) {
            pending_space = written_word;
            ++read_ptr;
        }

        word_start = read_ptr;

        while (*read_ptr != '\0' &&
               isspace((unsigned char)*read_ptr) == 0) {
            ++read_ptr;
        }

        word_length = (size_t)(read_ptr - word_start);

        if (word_length != 0U && word_length != k) {
            if (pending_space != 0) {
                *write_ptr++ = ' ';
            }

            memmove(write_ptr, word_start, word_length);
            write_ptr += word_length;
            written_word = 1;
            pending_space = 0;
        }
    }

    *write_ptr = '\0';
    return 0;
}

int main(int argc, char *argv[])
{
    char *end_ptr;
    unsigned long parsed_length;
    size_t k;
    char *string;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <k> <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    errno = 0;
    end_ptr = NULL;
    parsed_length = strtoul(argv[1], &end_ptr, 10);

    if (errno != 0 || end_ptr == argv[1] || *end_ptr != '\0' ||
        parsed_length > SIZE_MAX) {
        fprintf(stderr, "Invalid word length\n");
        return EXIT_FAILURE;
    }

    k = (size_t)parsed_length;
    string = malloc(strlen(argv[2]) + 1U);

    if (string == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    strcpy(string, argv[2]);

    if (remove_words_of_length(string, k) != 0) {
        fprintf(stderr, "Failed to process the string\n");
        free(string);
        return EXIT_FAILURE;
    }

    puts(string);
    free(string);
    return EXIT_SUCCESS;
}