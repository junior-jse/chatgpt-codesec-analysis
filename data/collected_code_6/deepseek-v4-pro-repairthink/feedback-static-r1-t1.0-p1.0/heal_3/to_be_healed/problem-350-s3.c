#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_line(void) {
    size_t cap = 128;
    size_t len = 0;
    char* buf = malloc(cap);
    if (!buf) return NULL;
    int c;
    /* Possible weaknesses found:
     * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((c = getchar()) != EOF && c != '\n') {
        if (len + 1 >= cap) {
            size_t newcap = cap * 2;
            char* newbuf = realloc(buf, newcap);
            if (!newbuf) {
                free(buf);
                return NULL;
            }
            buf = newbuf;
            cap = newcap;
        }
        buf[len++] = (char)c;
    }
    if (c == EOF && len == 0) {
        free(buf);
        return NULL;
    }
    buf[len] = '\0';
    return buf;
}

char* minimize_length(char* str, size_t len) {
    if (str == NULL || len == 0) {
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }

    int freq[256] = {0};
    for (size_t i = 0; i < len; i++) {
        freq[(unsigned char)str[i]]++;
    }

    int max_freq = -1;
    char max_char = '\0';
    for (int i = 0; i < 256; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            max_char = (char)i;
        }
    }

    char* result = malloc(len + 1);
    if (!result) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] != max_char) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    char* line = read_line();
    if (!line) return 1;

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(line);

    char* minimized = minimize_length(line, len);
    if (!minimized) {
        free(line);
        return 1;
    }

    printf("%s\n", minimized);
    free(minimized);
    free(line);
    return 0;
}