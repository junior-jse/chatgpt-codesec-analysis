#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char *read_line(void) {
    size_t size = 128;
    char *buffer = malloc(size);
    if (!buffer) return NULL;
    size_t len = 0;
    int c;

    while ((c = fgetc(stdin)) != EOF && c != '\n') {
        if (len + 1 >= size) {
            if (size > (SIZE_MAX - 1) / 2) {
                free(buffer);
                return NULL;
            }
            size *= 2;
            char *newbuf = realloc(buffer, size);
            if (!newbuf) {
                free(buffer);
                return NULL;
            }
            buffer = newbuf;
        }
        buffer[len++] = (char)c;
    }
    if (c == EOF && len == 0) {
        free(buffer);
        return NULL;
    }
    buffer[len] = '\0';
    return buffer;
}

int min_swaps_to_convert(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return -1;

    size_t i = 0;
    int count_s1 = 0, count_s2 = 0;
    int mismatches = 0, ones_mismatch = 0;

    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != '0' && s1[i] != '1') return -1;
        if (s2[i] != '0' && s2[i] != '1') return -1;

        if (s1[i] == '1') count_s1++;
        if (s2[i] == '1') count_s2++;
        if (s1[i] != s2[i]) {
            mismatches++;
            if (s1[i] == '1') ones_mismatch++;
        }
        i++;
    }
    if (s1[i] != '\0' || s2[i] != '\0') return -1;
    if (count_s1 != count_s2) return -1;

    int zeros_mismatch = mismatches - ones_mismatch;
    return ones_mismatch < zeros_mismatch ? ones_mismatch : zeros_mismatch;
}

int main(void) {
    char *s1 = NULL;
    char *s2 = NULL;

    printf("Enter first binary string: ");
    s1 = read_line();
    if (s1 == NULL) return 1;

    printf("Enter second binary string: ");
    s2 = read_line();
    if (s2 == NULL) {
        free(s1);
        return 1;
    }

    int result = min_swaps_to_convert(s1, s2);
    printf("%d\n", result);

    free(s1);
    free(s2);
    return 0;
}