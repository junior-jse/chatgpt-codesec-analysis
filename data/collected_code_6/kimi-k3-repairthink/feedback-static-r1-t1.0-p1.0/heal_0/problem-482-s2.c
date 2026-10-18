#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

void find_sequences(const char *str) {
    regex_t regex;
    regmatch_t match;
    const char *pattern = "[A-Z][a-z]+";
    const char *cursor = str;
    int ret;

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        char error_buffer[256];
        regerror(ret, &regex, error_buffer, sizeof(error_buffer));
        fprintf(stderr, "Could not compile regex: %s\n", error_buffer);
        return;
    }

    while (1) {
        ret = regexec(&regex, cursor, 1, &match, 0);
        if (ret == REG_NOMATCH) {
            break;
        } else if (ret != 0) {
            char error_buffer[256];
            regerror(ret, &regex, error_buffer, sizeof(error_buffer));
            fprintf(stderr, "Regex match failed: %s\n", error_buffer);
            break;
        }

        printf("Found sequence: %.*s\n", (int)(match.rm_eo - match.rm_so), cursor + match.rm_so);
        cursor += match.rm_eo;
    }

    regfree(&regex);
}

int main(void) {
    const char *test_string = "Hello World This Is A Test String With Sequences Like Abcdef And Xyz";
    find_sequences(test_string);
    return 0;
}