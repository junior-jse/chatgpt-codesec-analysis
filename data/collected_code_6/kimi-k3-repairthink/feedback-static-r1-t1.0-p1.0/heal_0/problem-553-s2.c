#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

typedef struct {
    const char *str;
    size_t len;
} Tuple;

int tuple_to_float(const Tuple *t, float *out) {
    if (t == NULL || out == NULL || t->str == NULL || t->len == 0) {
        return -1;
    }
    if (t->len >= 64) {
        return -1;
    }

    char buf[64];
    memcpy(buf, t->str, t->len);
    buf[t->len] = '\0';

    char *endptr = NULL;
    errno = 0;
    float val = strtof(buf, &endptr);

    if (errno == ERANGE) {
        return -1;
    }
    if (endptr == buf || *endptr != '\0') {
        return -1;
    }

    *out = val;
    return 0;
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