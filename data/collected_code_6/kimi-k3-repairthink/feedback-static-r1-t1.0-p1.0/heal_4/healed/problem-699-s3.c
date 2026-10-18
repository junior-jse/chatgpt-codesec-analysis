#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

static size_t bounded_strlen(const char *s, size_t max_len)
{
    size_t n = 0;

    if (s == NULL) {
        return 0;
    }

    while (n < max_len && s[n] != '\0') {
        n++;
    }

    return n;
}

static char *read_line(FILE *stream)
{
    size_t capacity = 128;
    size_t length = 0;
    char *buffer;
    int c;

    if (stream == NULL) {
        return NULL;
    }

    buffer = malloc(capacity);
    if (buffer == NULL) {
        return NULL;
    }

    while ((c = fgetc(stream)) != EOF && c != '\n') {
        if (length + 1 >= capacity) {
            size_t new_capacity;
            char *new_buffer;

            if (capacity > SIZE_MAX / 2) {
                free(buffer);
                return NULL;
            }

            new_capacity = capacity * 2;
            new_buffer = realloc(buffer, new_capacity);
            if (new_buffer == NULL) {
                free(buffer);
                return NULL;
            }

            buffer = new_buffer;
            capacity = new_capacity;
        }

        if (length + 1 < capacity) {
            buffer[length] = (char)c;
            length++;
        } else {
            free(buffer);
            return NULL;
        }
    }

    if (ferror(stream)) {
        free(buffer);
        return NULL;
    }

    if (c == EOF && length == 0) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

int min_swaps(const char *s1, const char *s2, size_t max_len)
{
    size_t len1;
    size_t len2;
    size_t i;
    size_t count1_s1 = 0;
    size_t count1_s2 = 0;
    long swaps = 0;
    long diff = 0;

    if (s1 == NULL || s2 == NULL) {
        return -1;
    }

    len1 = bounded_strlen(s1, max_len);
    len2 = bounded_strlen(s2, max_len);

    if (len1 != len2) {
        return -1;
    }

    for (i = 0; i < len1; i++) {
        if (s1[i] != '0' && s1[i] != '1') {
            return -1;
        }
        if (s2[i] != '0' && s2[i] != '1') {
            return -1;
        }
        if (s1[i] == '1') {
            count1_s1++;
        }
        if (s2[i] == '1') {
            count1_s2++;
        }
    }

    if (count1_s1 != count1_s2) {
        return -1;
    }

    for (i = 0; i < len1; i++) {
        if (s1[i] != s2[i]) {
            if (s1[i] == '1') {
                diff++;
            } else {
                diff--;
            }
            if (diff > 0) {
                swaps += diff;
                if (swaps > INT_MAX) {
                    swaps = INT_MAX;
                }
            }
        }
    }

    return (int)swaps;
}

int main(void)
{
    char *s1;
    char *s2;
    int result;

    s1 = read_line(stdin);
    if (s1 == NULL) {
        return 1;
    }

    s2 = read_line(stdin);
    if (s2 == NULL) {
        free(s1);
        return 1;
    }

    result = min_swaps(s1, s2, SIZE_MAX);
    printf("%d\n", result);

    free(s1);
    free(s2);

    return 0;
}