#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 1000
#define MAX_LENGTH 100

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char str[MAX_LENGTH];
    int count;
} StringCount;

static size_t safe_str_copy(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) return 0;
    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return i;
}

static char *duplicate_string(const char *src) {
    if (src == NULL) return NULL;
    size_t len = strnlen(src, MAX_LENGTH);
    char *dest = (char *)malloc(len + 1);
    if (dest == NULL) return NULL;
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dest, src, len);
    dest[len] = '\0';
    return dest;
}

char *second_most_repeated(const char strings[][MAX_LENGTH], int n) {
    if (n <= 0 || strings == NULL) return NULL;
    if (n > MAX_STRINGS) return NULL;

    StringCount *counts = (StringCount *)calloc((size_t)n, sizeof(StringCount));
    if (counts == NULL) return NULL;

    int unique_count = 0;

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strncmp(counts[j].str, strings[i], MAX_LENGTH - 1) == 0) {
                counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (unique_count >= n) break;
            safe_str_copy(counts[unique_count].str, strings[i], MAX_LENGTH);
            counts[unique_count].count = 1;
            unique_count++;
        }
    }

    if (unique_count < 2) {
        free(counts);
        return NULL;
    }

    int first_max = 0, second_max = -1;

    for (int i = 1; i < unique_count; i++) {
        if (counts[i].count > counts[first_max].count) {
            second_max = first_max;
            first_max = i;
        } else if (counts[i].count < counts[first_max].count) {
            if (second_max == -1 || counts[i].count > counts[second_max].count) {
                second_max = i;
            }
        }
    }

    if (second_max == -1) {
        free(counts);
        return NULL;
    }

    char *result = duplicate_string(counts[second_max].str);

    free(counts);
    return result;
}

int main() {
    char strings[][MAX_LENGTH] = {
        "apple", "banana", "apple", "orange", "banana", "apple", "grape"
    };
    int n = sizeof(strings) / sizeof(strings[0]);

    char *result = second_most_repeated(strings, n);

    if (result != NULL) {
        printf("Second most repeated string: %s\n", result);
        free(result);
    } else {
        printf("No second most repeated string found.\n");
    }

    return 0;
}