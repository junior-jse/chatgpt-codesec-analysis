#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int first;
    char *second;
    double third;
} Tuple;

char *flatten_tuple_list(Tuple *tuples, int count) {
    if (count <= 0) {
        char *empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }

    size_t total_len = 0;
    size_t *lens = malloc((size_t)count * sizeof(size_t));
    if (!lens) return NULL;

    for (int i = 0; i < count; i++) {
        int ilen = snprintf(NULL, 0, "%d", tuples[i].first);
        int dlen = snprintf(NULL, 0, "%.2f", tuples[i].third);
        size_t slen = tuples[i].second ? strnlen(tuples[i].second, SIZE_MAX) : 0;
        lens[i] = (size_t)ilen + (size_t)dlen + slen;
        total_len += lens[i];
    }

    size_t total_sep = (size_t)(count - 1);
    size_t bufsize = total_len + total_sep + 1;

    char *result = malloc(bufsize);
    if (!result) {
        free(lens);
        return NULL;
    }

    char *ptr = result;
    for (int i = 0; i < count; i++) {
        size_t remaining = bufsize - (size_t)(ptr - result);
        int written = snprintf(ptr, remaining, "%d%s%.2f",
                               tuples[i].first,
                               tuples[i].second ? tuples[i].second : "",
                               tuples[i].third);
        if (written < 0 || (size_t)written >= remaining) {
            free(result);
            free(lens);
            return NULL;
        }
        ptr += written;
        if (i < count - 1) {
            *ptr++ = ',';
        }
    }
    *ptr = '\0';

    free(lens);
    return result;
}

int main(void) {
    Tuple tuples[] = {
        {1, "abc", 3.14},
        {2, "def", 2.71},
        {3, "ghi", 1.41}
    };
    int count = (int)(sizeof(tuples) / sizeof(tuples[0]));

    char *flat = flatten_tuple_list(tuples, count);
    if (flat) {
        printf("%s\n", flat);
        free(flat);
    }

    return 0;
}