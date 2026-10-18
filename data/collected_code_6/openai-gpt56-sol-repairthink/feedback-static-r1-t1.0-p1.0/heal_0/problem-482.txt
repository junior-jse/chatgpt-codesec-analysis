#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int find_sequences(const char *input)
{
    regex_t regex;
    regmatch_t match;
    const char *pattern = "[[:upper:]][[:lower:]]+";
    const char *cursor;
    int status;

    if (input == NULL) {
        return -1;
    }

    status = regcomp(&regex, pattern, REG_EXTENDED);
    if (status != 0) {
        char error_message[256];
        regerror(status, &regex, error_message, sizeof(error_message));
        fprintf(stderr, "regcomp failed: %s\n", error_message);
        return -1;
    }

    cursor = input;

    while ((status = regexec(&regex, cursor, 1, &match, 0)) == 0) {
        size_t length;

        if (match.rm_so < 0 || match.rm_eo <= match.rm_so) {
            regfree(&regex);
            return -1;
        }

        length = (size_t)(match.rm_eo - match.rm_so);

        if (fwrite(cursor + match.rm_so, 1, length, stdout) != length ||
            fputc('\n', stdout) == EOF) {
            regfree(&regex);
            return -1;
        }

        cursor += match.rm_eo;
    }

    if (status != REG_NOMATCH) {
        char error_message[256];
        regerror(status, &regex, error_message, sizeof(error_message));
        fprintf(stderr, "regexec failed: %s\n", error_message);
        regfree(&regex);
        return -1;
    }

    regfree(&regex);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"string\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (find_sequences(argv[1]) != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}