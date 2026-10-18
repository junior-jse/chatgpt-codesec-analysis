#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

int replace_delimiters(char *str)
{
    regex_t regex;
    regmatch_t match;
    char *cursor;
    int result;

    if (str == NULL) {
        return -1;
    }

    result = regcomp(&regex, "[ ,.]", REG_EXTENDED);
    if (result != 0) {
        return -1;
    }

    cursor = str;

    while ((result = regexec(&regex, cursor, 1, &match, 0)) == 0) {
        if (match.rm_so < 0 || match.rm_eo <= match.rm_so) {
            regfree(&regex);
            return -1;
        }

        cursor[match.rm_so] = ':';
        cursor += match.rm_so + 1;
    }

    regfree(&regex);

    if (result != REG_NOMATCH) {
        return -1;
    }

    return 0;
}

int main(void)
{
    char text[] = "Hello, world. This is a test string.";

    if (replace_delimiters(text) != 0) {
        fputs("Failed to replace delimiters.\n", stderr);
        return EXIT_FAILURE;
    }

    puts(text);
    return EXIT_SUCCESS;
}