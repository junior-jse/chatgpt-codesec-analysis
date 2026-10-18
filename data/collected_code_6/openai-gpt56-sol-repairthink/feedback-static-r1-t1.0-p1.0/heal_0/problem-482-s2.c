#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

static int find_sequences(const char *input)
{
    regex_t regex;
    regmatch_t match;
    const char *cursor;
    int status;

    if (input == NULL) {
        return -1;
    }

    status = regcomp(&regex, "[[:upper:]][[:lower:]]+", REG_EXTENDED);
    if (status != 0) {
        char error_message[256];

        regerror(status, &regex, error_message, sizeof(error_message));
        fprintf(stderr, "regcomp failed: %s\n", error_message);
        return -1;
    }

    cursor = input;

    while (*cursor != '\0') {
        status = regexec(&regex, cursor, 1, &match, 0);

        if (status == REG_NOMATCH) {
            break;
        }

        if (status != 0) {
            char error_message[256];

            regerror(status, &regex, error_message, sizeof(error_message));
            fprintf(stderr, "regexec failed: %s\n", error_message);
            regfree(&regex);
            return -1;
        }

        if (match.rm_so < 0 || match.rm_eo <= match.rm_so) {
            fprintf(stderr, "Invalid regular expression match\n");
            regfree(&regex);
            return -1;
        }

        if (printf("%.*s\n",
                   (int)(match.rm_eo - match.rm_so),
                   cursor + match.rm_so) < 0) {
            regfree(&regex);
            return -1;
        }

        cursor += match.rm_eo;
    }

    regfree(&regex);
    return 0;
}

int main(int argc, char *argv[])
{
    size_t total_length = 0;
    char *input;
    char *destination;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <text>\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; ++i) {
        size_t length = strlen(argv[i]);

        if (length > SIZE_MAX - total_length - 1U) {
            fprintf(stderr, "Input is too large\n");
            return EXIT_FAILURE;
        }

        total_length += length;

        if (i + 1 < argc) {
            if (total_length == SIZE_MAX) {
                fprintf(stderr, "Input is too large\n");
                return EXIT_FAILURE;
            }
            ++total_length;
        }
    }

    input = malloc(total_length + 1U);
    if (input == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    destination = input;

    for (int i = 1; i < argc; ++i) {
        size_t length = strlen(argv[i]);

        memcpy(destination, argv[i], length);
        destination += length;

        if (i + 1 < argc) {
            *destination++ = ' ';
        }
    }

    *destination = '\0';

    if (find_sequences(input) != 0) {
        free(input);
        return EXIT_FAILURE;
    }

    free(input);
    return EXIT_SUCCESS;
}