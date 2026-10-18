#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    const char *value;
    size_t count;
    size_t first_index;
} StringFrequency;

static int compare_frequencies(const void *left, const void *right)
{
    const StringFrequency *a = left;
    const StringFrequency *b = right;

    if (a->count != b->count) {
        return a->count < b->count ? 1 : -1;
    }

    if (a->first_index != b->first_index) {
        return a->first_index > b->first_index ? 1 : -1;
    }

    return 0;
}

static const char *find_second_most_frequent(const char *const strings[],
                                             size_t count)
{
    StringFrequency *frequencies;
    size_t unique_count = 0U;
    size_t i;

    if (strings == NULL || count < 2U ||
        count > SIZE_MAX / sizeof(*frequencies)) {
        return NULL;
    }

    frequencies = calloc(count, sizeof(*frequencies));
    if (frequencies == NULL) {
        return NULL;
    }

    for (i = 0U; i < count; ++i) {
        size_t j;

        if (strings[i] == NULL) {
            free(frequencies);
            return NULL;
        }

        for (j = 0U; j < unique_count; ++j) {
            if (strcmp(strings[i], frequencies[j].value) == 0) {
                ++frequencies[j].count;
                break;
            }
        }

        if (j == unique_count) {
            frequencies[unique_count].value = strings[i];
            frequencies[unique_count].count = 1U;
            frequencies[unique_count].first_index = i;
            ++unique_count;
        }
    }

    if (unique_count < 2U) {
        free(frequencies);
        return NULL;
    }

    qsort(frequencies, unique_count, sizeof(*frequencies),
          compare_frequencies);

    {
        const char *result = frequencies[1].value;
        free(frequencies);
        return result;
    }
}

int main(void)
{
    const char *strings[] = {
        "apple", "banana", "apple", "orange",
        "banana", "apple", "orange", "banana", "orange", "orange"
    };
    const size_t count = sizeof(strings) / sizeof(strings[0]);
    const char *result = find_second_most_frequent(strings, count);

    if (result == NULL) {
        fputs("No second most frequent string found.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s\n", result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}