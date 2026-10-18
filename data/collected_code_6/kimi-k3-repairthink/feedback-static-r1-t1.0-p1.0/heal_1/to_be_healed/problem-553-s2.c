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

    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buf[64];
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
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