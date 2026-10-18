#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>

typedef struct {
    int first;
    int second;
    int third;
} ClosestElements;

static long long minimum3(long long a, long long b, long long c)
{
    long long minimum = a < b ? a : b;
    return minimum < c ? minimum : c;
}

static long long maximum3(long long a, long long b, long long c)
{
    long long maximum = a > b ? a : b;
    return maximum > c ? maximum : c;
}

static int find_three_closest(const int first[], size_t first_size,
                              const int second[], size_t second_size,
                              const int third[], size_t third_size,
                              ClosestElements *result)
{
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    long long best_range = LLONG_MAX;

    if (first == NULL || second == NULL || third == NULL || result == NULL ||
        first_size == 0 || second_size == 0 || third_size == 0) {
        return -1;
    }

    while (i < first_size && j < second_size && k < third_size) {
        long long a = first[i];
        long long b = second[j];
        long long c = third[k];
        long long minimum = minimum3(a, b, c);
        long long maximum = maximum3(a, b, c);
        long long range = maximum - minimum;

        if (range < best_range) {
            best_range = range;
            result->first = first[i];
            result->second = second[j];
            result->third = third[k];

            if (best_range == 0) {
                return 0;
            }
        }

        if (a == minimum) {
            ++i;
        } else if (b == minimum) {
            ++j;
        } else {
            ++k;
        }
    }

    return 0;
}

int main(void)
{
    const int first[] = {1, 4, 10};
    const int second[] = {2, 15, 20};
    const int third[] = {10, 12};
    const size_t first_size = sizeof(first) / sizeof(first[0]);
    const size_t second_size = sizeof(second) / sizeof(second[0]);
    const size_t third_size = sizeof(third) / sizeof(third[0]);
    ClosestElements result = {0, 0, 0};

    if (find_three_closest(first, first_size, second, second_size,
                           third, third_size, &result) != 0) {
        fputs("Unable to find closest elements.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%d %d %d\n", result.first, result.second, result.third) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}