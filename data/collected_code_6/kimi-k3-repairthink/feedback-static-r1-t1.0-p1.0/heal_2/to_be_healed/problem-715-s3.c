#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define MAX_STR_LEN 4096

typedef struct {
    int *data;
    size_t size;
} Tuple;

Tuple string_to_tuple(const char *str) {
    Tuple t = {NULL, 0};
    if (str == NULL) {
        return t;
    }

    size_t len = strnlen(str, MAX_STR_LEN);
    if (len == 0 || len >= MAX_STR_LEN) {
        return t;
    }

    size_t count = 1;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == ',') {
            count++;
        }
    }

    t.data = malloc(count * sizeof(int));
    if (t.data == NULL) {
        return t;
    }
    t.size = count;

    char *copy = malloc(len + 1);
    if (copy == NULL) {
        free(t.data);
        t.data = NULL;
        t.size = 0;
        return t;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, str, len + 1);

    size_t i = 0;
    char *saveptr = NULL;
    char *token = strtok_r(copy, ",", &saveptr);
    while (token != NULL && i < t.size) {
        errno = 0;
        char *endptr = NULL;
        long value = strtol(token, &endptr, 10);
        if (errno != 0 || endptr == token || *endptr != '\0' ||
            value < INT_MIN || value > INT_MAX) {
            free(copy);
            free(t.data);
            t.data = NULL;
            t.size = 0;
            return t;
        }
        t.data[i] = (int)value;
        i++;
        token = strtok_r(NULL, ",", &saveptr);
    }

    free(copy);

    if (i != t.size) {
        free(t.data);
        t.data = NULL;
        t.size = 0;
        return t;
    }

    return t;
}

void free_tuple(Tuple *t) {
    if (t != NULL) {
        free(t->data);
        t->data = NULL;
        t->size = 0;
    }
}

int main(void) {
    const char *input = "1,2,3,4,5";
    Tuple t = string_to_tuple(input);

    if (t.data != NULL) {
        printf("Tuple: (");
        for (size_t i = 0; i < t.size; i++) {
            printf("%d", t.data[i]);
            if (i < t.size - 1) {
                printf(", ");
            }
        }
        printf(")\n");
        free_tuple(&t);
    } else {
        printf("Failed to create tuple\n");
    }

    return 0;
}