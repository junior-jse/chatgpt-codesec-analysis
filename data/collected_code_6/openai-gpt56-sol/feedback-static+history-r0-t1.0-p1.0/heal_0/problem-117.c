#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const char *text;
    float value;
    int converted;
} Element;

static int convert_to_float(const char *text, float *value)
{
    char *end = NULL;
    float result;

    if (text == NULL || value == NULL) {
        return 0;
    }

    errno = 0;
    result = strtof(text, &end);

    if (end == text || *end != '\0' || errno == ERANGE || !isfinite(result)) {
        return 0;
    }

    *value = result;
    return 1;
}

static size_t convert_elements(Element *elements, size_t count)
{
    size_t converted_count = 0;

    if (elements == NULL) {
        return 0;
    }

    for (size_t i = 0; i < count; ++i) {
        elements[i].converted = convert_to_float(elements[i].text, &elements[i].value);
        if (elements[i].converted) {
            ++converted_count;
        }
    }

    return converted_count;
}

int main(void)
{
    Element elements[] = {
        {"12.5", 0.0F, 0},
        {"-3.25", 0.0F, 0},
        {"invalid", 0.0F, 0},
        {"1e4", 0.0F, 0},
        {"", 0.0F, 0}
    };
    const size_t count = sizeof(elements) / sizeof(elements[0]);
    const size_t converted_count = convert_elements(elements, count);

    for (size_t i = 0; i < count; ++i) {
        if (elements[i].converted) {
            if (printf("%s -> %.9g\n", elements[i].text, (double)elements[i].value) < 0) {
                return EXIT_FAILURE;
            }
        } else {
            if (printf("%s -> not convertible\n", elements[i].text) < 0) {
                return EXIT_FAILURE;
            }
        }
    }

    if (printf("Converted: %zu of %zu\n", converted_count, count) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}