#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t distance_i64(int64_t minimum, int64_t maximum)
{
    return (uint64_t)maximum - (uint64_t)minimum;
}

static int find_closest_elements(
    const int64_t *first,
    size_t first_size,
    const int64_t *second,
    size_t second_size,
    const int64_t *third,
    size_t third_size,
    int64_t *first_result,
    int64_t *second_result,
    int64_t *third_result)
{
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    uint64_t best_distance = UINT64_MAX;

    if (first == NULL || second == NULL || third == NULL ||
        first_result == NULL || second_result == NULL || third_result == NULL ||
        first_size == 0 || second_size == 0 || third_size == 0) {
        return -1;
    }

    while (i < first_size && j < second_size && k < third_size) {
        int64_t minimum = first[i];
        int64_t maximum = first[i];

        if (second[j] < minimum) {
            minimum = second[j];
        }
        if (third[k] < minimum) {
            minimum = third[k];
        }
        if (second[j] > maximum) {
            maximum = second[j];
        }
        if (third[k] > maximum) {
            maximum = third[k];
        }

        const uint64_t current_distance = distance_i64(minimum, maximum);

        if (current_distance < best_distance) {
            best_distance = current_distance;
            *first_result = first[i];
            *second_result = second[j];
            *third_result = third[k];

            if (best_distance == 0) {
                return 0;
            }
        }

        if (first[i] == minimum) {
            ++i;
        } else if (second[j] == minimum) {
            ++j;
        } else {
            ++k;
        }
    }

    return 0;
}

int main(void)
{
    static const int64_t first[] = {1, 4, 10};
    static const int64_t second[] = {2, 15, 20};
    static const int64_t third[] = {10, 12};
    int64_t first_result = 0;
    int64_t second_result = 0;
    int64_t third_result = 0;

    if (find_closest_elements(
            first,
            sizeof(first) / sizeof(first[0]),
            second,
            sizeof(second) / sizeof(second[0]),
            third,
            sizeof(third) / sizeof(third[0]),
            &first_result,
            &second_result,
            &third_result) != 0) {
        fputs("Failed to find closest elements.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf(
            "Closest elements: %" PRId64 ", %" PRId64 ", %" PRId64 "\n",
            first_result,
            second_result,
            third_result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}