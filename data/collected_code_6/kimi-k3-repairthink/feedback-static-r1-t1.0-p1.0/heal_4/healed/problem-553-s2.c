#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>

typedef struct {
    const char *str;
    size_t len;
} Tuple;

int tuple_to_float(const Tuple *t, float *out) {
    if (t == NULL || out == NULL || t->str == NULL || t->len == 0) {
        return -1;
    }
    if (t->len >= SIZE_MAX) {
        return -1;
    }

    const char *parse_str = t->str;
    char *buf = NULL;

    if (strnlen(t->str, t->len) == t->len) {
        size_t buf_size = t->len + 1;
        if (buf_size <= t->len) {
            return -1;
        }
        buf = malloc(buf_size);
        if (buf == NULL) {
            return -1;
        }
        if (t->len < buf_size) {
            memcpy(buf, t->str, t->len);
            buf[t->len] = '\0';
            parse_str = buf;
        } else {
            free(buf);
            return -1;
        }
    }

    char *endptr = NULL;
    errno = 0;
    float val = strtof(parse_str, &endptr);

    int ret = 0;
    if (errno == ERANGE || endptr == parse_str || *endptr != '\0') {
        ret = -1;
    } else {
        *out = val;
    }

    free(buf);
    return ret;
}

int main(void) {
    Tuple t = {"3.14159", 7};
    float result = 0.0f;

    if (tuple_to_float(&t, &result) == 0) {
        printf("Converted value: %f\n", (double)result);
    } else {
        fprintf(stderr, "Conversion failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}