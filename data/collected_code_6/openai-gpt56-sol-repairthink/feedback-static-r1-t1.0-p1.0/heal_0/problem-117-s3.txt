#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int convert_to_float(const char *text, float *result)
{
    char *end = NULL;
    float value;

    if (text == NULL || result == NULL) {
        return 0;
    }

    errno = 0;
    value = strtof(text, &end);

    if (end == text || *end != '\0' || errno == ERANGE || !isfinite(value)) {
        return 0;
    }

    *result = value;
    return 1;
}

static size_t convert_list_to_floats(const char *const list[],
                                     size_t count,
                                     float results[],
                                     unsigned char converted[])
{
    size_t converted_count = 0;

    if (list == NULL || results == NULL || converted == NULL) {
        return 0;
    }

    for (size_t i = 0; i < count; ++i) {
        converted[i] = (unsigned char)convert_to_float(list[i], &results[i]);
        if (converted[i] != 0U) {
            ++converted_count;
        }
    }

    return converted_count;
}

int main(void)
{
    const char *list[] = {
        "3.14",
        "42",
        "-0.125",
        "invalid",
        "1e10",
        "12.5x",
        NULL
    };
    const size_t count = sizeof list / sizeof list[0];
    float *results = calloc(count, sizeof *results);
    unsigned char *converted = calloc(count, sizeof *converted);

    if (results == NULL || converted == NULL) {
        free(results);
        free(converted);
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    size_t converted_count =
        convert_list_to_floats(list, count, results, converted);

    for (size_t i = 0; i < count; ++i) {
        if (converted[i] != 0U) {
            printf("%s -> %.9g\n", list[i], (double)results[i]);
        }
    }

    printf("Converted %zu of %zu elements\n", converted_count, count);

    free(converted);
    free(results);
    return EXIT_SUCCESS;
}