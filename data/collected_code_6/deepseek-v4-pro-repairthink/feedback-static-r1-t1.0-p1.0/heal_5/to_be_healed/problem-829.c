#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *str;
    int count;
} StringCount;

static char *my_strdup(const char *s) {
    if (!s) return NULL;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(s);
    char *p = malloc(len + 1);
    if (p) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(p, s, len);
        p[len] = '\0';
    }
    return p;
}

char *second_most_frequent(const char *strings[], int n) {
    if (n <= 0) return NULL;

    StringCount *counts = malloc(n * sizeof(StringCount));
    if (!counts) return NULL;

    int unique_count = 0;
    int cleanup_needed = 0;

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(counts[j].str, strings[i]) == 0) {
                counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            counts[unique_count].str = my_strdup(strings[i]);
            if (!counts[unique_count].str) {
                cleanup_needed = 1;
                break;
            }
            counts[unique_count].count = 1;
            unique_count++;
        }
    }

    if (cleanup_needed) {
        for (int k = 0; k < unique_count; k++) {
            free(counts[k].str);
        }
        free(counts);
        return NULL;
    }

    if (unique_count < 2) {
        for (int k = 0; k < unique_count; k++) {
            free(counts[k].str);
        }
        free(counts);
        return NULL;
    }

    int first_max = 0;
    int second_max = -1;

    for (int i = 1; i < unique_count; i++) {
        if (counts[i].count > counts[first_max].count) {
            second_max = first_max;
            first_max = i;
        } else if (second_max == -1 || counts[i].count > counts[second_max].count) {
            if (counts[i].count != counts[first_max].count) {
                second_max = i;
            }
        }
    }

    char *result = NULL;
    if (second_max != -1) {
        result = my_strdup(counts[second_max].str);
    }

    for (int k = 0; k < unique_count; k++) {
        free(counts[k].str);
    }
    free(counts);

    return result;
}

static char *read_line(void) {
    size_t capacity = 16;
    size_t length = 0;
    char *buffer = malloc(capacity);
    if (!buffer) return NULL;

    int c;
    /* Possible weaknesses found:
     * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((c = getchar()) != EOF && c != '\n') {
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *new_buf = realloc(buffer, capacity);
            if (!new_buf) {
                free(buffer);
                return NULL;
            }
            buffer = new_buf;
        }
        buffer[length++] = (char)c;
    }

    if (c == EOF && length == 0) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

int main(void) {
    int n;
    printf("Enter number of strings: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input\n");
        return 1;
    }

    int c;
    /* Possible weaknesses found:
     * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((c = getchar()) != '\n' && c != EOF) {}

    const char **strings = malloc(n * sizeof(const char *));
    if (!strings) {
        printf("Memory allocation error\n");
        return 1;
    }

    printf("Enter %d strings:\n", n);
    for (int i = 0; i < n; i++) {
        strings[i] = read_line();
        if (!strings[i]) {
            strings[i] = malloc(1);
            if (strings[i]) {
                /* Possible weaknesses found:
                 * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
                 */
                ((char *)strings[i])[0] = '\0';
            } else {
                for (int j = 0; j < i; j++) {
                    free((void *)strings[j]);
                }
                free(strings);
                printf("Memory allocation error\n");
                return 1;
            }
        }
    }

    char *result = second_most_frequent(strings, n);

    if (result) {
        printf("Second most frequent string: %s\n", result);
        free(result);
    } else {
        printf("No second most frequent string found\n");
    }

    for (int i = 0; i < n; i++) {
        free((void *)strings[i]);
    }
    free(strings);

    return 0;
}