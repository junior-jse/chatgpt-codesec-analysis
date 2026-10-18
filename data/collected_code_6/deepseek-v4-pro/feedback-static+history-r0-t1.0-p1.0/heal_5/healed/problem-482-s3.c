#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

void find_sequences(const char *str) {
    regex_t regex;
    regmatch_t match[1];
    const char *pattern = "[A-Z][a-z]+";
    int ret;
    const char *cursor = str;
    size_t str_len;

    if (!str) {
        return;
    }

    str_len = strlen(str);
    if (str_len == 0) {
        return;
    }

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) {
        fprintf(stderr, "Could not compile regex\n");
        return;
    }

    while (regexec(&regex, cursor, 1, match, 0) == 0) {
        int start = match[0].rm_so;
        int end = match[0].rm_eo;
        size_t cursor_offset;

        if (start < 0 || end < 0 || start >= end) {
            cursor++;
            continue;
        }

        cursor_offset = (size_t)(cursor - str);

        if (cursor_offset + (size_t)start >= str_len ||
            cursor_offset + (size_t)end > str_len ||
            (size_t)(end - start) > str_len - (cursor_offset + (size_t)start)) {
            break;
        }

        size_t len = (size_t)(end - start);
        char *seq = (char *)malloc(len + 1);
        if (!seq) {
            fprintf(stderr, "Memory allocation failed\n");
            regfree(&regex);
            return;
        }

        memcpy(seq, cursor + start, len);
        seq[len] = '\0';
        printf("%s\n", seq);
        free(seq);

        if ((size_t)end > str_len - cursor_offset) {
            break;
        }
        cursor += end;
    }

    regfree(&regex);
}

int main(void) {
    const char *text = "Hello World This Is A Test String with Some Uppercase Letters";
    find_sequences(text);
    return 0;
}