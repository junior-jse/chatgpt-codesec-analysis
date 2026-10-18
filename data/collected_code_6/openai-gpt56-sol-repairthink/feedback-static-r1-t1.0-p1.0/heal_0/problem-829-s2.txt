#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Frequency {
    const char *value;
    size_t count;
    size_t first_index;
};

static const char *second_most_frequent(const char *const strings[], size_t count)
{
    struct Frequency *frequencies;
    const char *result = NULL;
    size_t unique_count = 0;
    size_t second_index = SIZE_MAX;

    if (strings == NULL || count < 2) {
        return NULL;
    }

    frequencies = calloc(count, sizeof(*frequencies));
    if (frequencies == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        size_t j;

        if (strings[i] == NULL) {
            free(frequencies);
            return NULL;
        }

        for (j = 0; j < unique_count; ++j) {
            if (strcmp(strings[i], frequencies[j].value) == 0) {
                ++frequencies[j].count;
                break;
            }
        }

        if (j == unique_count) {
            frequencies[unique_count].value = strings[i];
            frequencies[unique_count].count = 1;
            frequencies[unique_count].first_index = i;
            ++unique_count;
        }
    }

    if (unique_count >= 2) {
        size_t highest_count = 0;
        size_t second_highest_count = 0;

        for (size_t i = 0; i < unique_count; ++i) {
            size_t current_count = frequencies[i].count;

            if (current_count > highest_count) {
                second_highest_count = highest_count;
                highest_count = current_count;
            } else if (current_count < highest_count &&
                       current_count > second_highest_count) {
                second_highest_count = current_count;
            }
        }

        if (second_highest_count > 0) {
            for (size_t i = 0; i < unique_count; ++i) {
                if (frequencies[i].count == second_highest_count &&
                    (second_index == SIZE_MAX ||
                     frequencies[i].first_index <
                         frequencies[second_index].first_index)) {
                    second_index = i;
                }
            }

            result = frequencies[second_index].value;
        }
    }

    free(frequencies);
    return result;
}

int main(void)
{
    const char *strings[] = {
        "apple", "banana", "apple", "orange",
        "banana", "apple", "orange", "banana",
        "apple", "grape"
    };
    size_t count = sizeof(strings) / sizeof(strings[0]);
    const char *result = second_most_frequent(strings, count);

    if (result == NULL) {
        fputs("No second most frequent string exists.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s\n", result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}