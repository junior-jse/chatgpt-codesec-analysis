#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    int is_valid;
    float value;
} ConversionResult;

ConversionResult *convert_to_floats(const char **list, size_t size) {
    if (list == NULL || size == 0) {
        return NULL;
    }

    ConversionResult *results = malloc(size * sizeof(ConversionResult));
    if (results == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        if (list[i] == NULL) {
            results[i].is_valid = 0;
            results[i].value = 0.0f;
            continue;
        }

        if (list[i][0] == '\0') {
            results[i].is_valid = 0;
            results[i].value = 0.0f;
            continue;
        }

        char *endptr = NULL;
        errno = 0;
        float val = strtof(list[i], &endptr);

        if (errno == ERANGE) {
            results[i].is_valid = 0;
            results[i].value = 0.0f;
        } else if (endptr == list[i]) {
            results[i].is_valid = 0;
            results[i].value = 0.0f;
        } else if (*endptr != '\0') {
            results[i].is_valid = 0;
            results[i].value = 0.0f;
        } else {
            results[i].is_valid = 1;
            results[i].value = val;
        }
    }

    return results;
}

int main(void) {
    const char *list[] = {
        "3.14",
        "-42.5",
        "0",
        "not_a_float",
        "123.456extra",
        "",
        "1e-5",
        NULL
    };

    size_t size = sizeof(list) / sizeof(list[0]);

    ConversionResult *results = convert_to_floats(list, size);
    if (results == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < size; i++) {
        if (results[i].is_valid) {
            printf("Valid: %f\n", results[i].value);
        } else {
            printf("Invalid conversion\n");
        }
    }

    free(results);
    results = NULL;

    return EXIT_SUCCESS;
}