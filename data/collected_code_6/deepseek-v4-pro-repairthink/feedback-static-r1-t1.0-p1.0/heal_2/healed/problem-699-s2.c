#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_line(void) {
    size_t size = 128;
    char *buffer = malloc(size);
    if (!buffer) return NULL;
    size_t len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (len + 1 >= size) {
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
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    if (len1 != len2) return -1;
    size_t len = len1;
    int count_s1 = 0, count_s2 = 0;
    int mismatches = 0, ones_mismatch = 0;
    for (size_t i = 0; i < len; i++) {
        if (s1[i] == '1') count_s1++;
        if (s2[i] == '1') count_s2++;
        if (s1[i] != s2[i]) {
            mismatches++;
            if (s1[i] == '1') ones_mismatch++;
        }
    }
    if (count_s1 != count_s2) return -1;
    return ones_mismatch < (mismatches - ones_mismatch) ? ones_mismatch : (mismatches - ones_mismatch);
}

int main(void) {
    char *s1, *s2;
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
    if (result == -1) {
        printf("-1\n");
    } else {
        printf("%d\n", result);
    }
    free(s1);
    free(s2);
    return 0;
}