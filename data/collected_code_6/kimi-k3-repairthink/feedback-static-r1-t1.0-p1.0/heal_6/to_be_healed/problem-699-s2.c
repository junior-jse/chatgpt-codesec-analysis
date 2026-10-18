#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define INITIAL_CAPACITY 16
#define MAX_STRING_LENGTH ((size_t)1 << 20)

static size_t bounded_strlen(const char *s, size_t maxlen) {
    size_t len = 0;

    while (len < maxlen && s[len] != '\0') {
        len++;
    }

    return len;
}

int min_swaps(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }

    size_t len1 = bounded_strlen(str1, MAX_STRING_LENGTH + 1);
    size_t len2 = bounded_strlen(str2, MAX_STRING_LENGTH + 1);

    if (len1 > MAX_STRING_LENGTH || len2 > MAX_STRING_LENGTH) {
        return -1;
    }

    if (len1 != len2) {
        return -1;
    }

    size_t count0_str1 = 0, count1_str1 = 0;
    size_t count0_str2 = 0, count1_str2 = 0;

    for (size_t i = 0; i < len1; i++) {
        if (str1[i] == '0') {
            count0_str1++;
        } else if (str1[i] == '1') {
            count1_str1++;
        } else {
            return -1;
        }

        if (str2[i] == '0') {
            count0_str2++;
        } else if (str2[i] == '1') {
            count1_str2++;
        } else {
            return -1;
        }
    }

    if (count0_str1 != count0_str2 || count1_str1 != count1_str2) {
        return -1;
    }

    size_t mismatches = 0;
    for (size_t i = 0; i < len1; i++) {
        if (str1[i] != str2[i]) {
            mismatches++;
        }
    }

    if (mismatches / 2 > (size_t)INT_MAX) {
        return -1;
    }

    return (int)(mismatches / 2);
}

char *read_line(FILE *stream) {
    if (stream == NULL) {
        return NULL;
    }

    size_t capacity = INITIAL_CAPACITY;
    size_t length = 0;
    char *buffer = malloc(capacity);

    if (buffer == NULL) {
        return NULL;
    }

    int c;
    /* Possible weaknesses found:
     * Flawfinder fgetc: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((c = fgetc(stream)) != EOF && c != '\n') {
        if (length + 1 >= capacity) {
            if (capacity >= MAX_STRING_LENGTH + 1 || capacity > SIZE_MAX / 2) {
                free(buffer);
                return NULL;
            }

            size_t new_capacity = capacity * 2;
            if (new_capacity < capacity || new_capacity > MAX_STRING_LENGTH + 1) {
                new_capacity = MAX_STRING_LENGTH + 1;
            }

            if (new_capacity <= capacity) {
                free(buffer);
                return NULL;
            }

            char *new_buffer = realloc(buffer, new_capacity);
            if (new_buffer == NULL) {
                free(buffer);
                return NULL;
            }

            buffer = new_buffer;
            capacity = new_capacity;
        }

        if (length + 1 >= capacity) {
            free(buffer);
            return NULL;
        }

        buffer[length++] = (char)c;
    }

    if (ferror(stream)) {
        free(buffer);
        return NULL;
    }

    if (c == EOF && length == 0) {
        free(buffer);
        return NULL;
    }

    if (length >= capacity) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

int main(void) {
    printf("Enter the first binary string: ");
    fflush(stdout);

    char *str1 = read_line(stdin);
    if (str1 == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }

    printf("Enter the second binary string: ");
    fflush(stdout);

    char *str2 = read_line(stdin);
    if (str2 == NULL) {
        fprintf(stderr, "Error reading input.\n");
        free(str1);
        return 1;
    }

    int result = min_swaps(str1, str2);

    if (result == -1) {
        printf("Conversion not possible.\n");
    } else {
        printf("Minimum number of swaps required: %d\n", result);
    }

    free(str1);
    free(str2);
    return 0;
}