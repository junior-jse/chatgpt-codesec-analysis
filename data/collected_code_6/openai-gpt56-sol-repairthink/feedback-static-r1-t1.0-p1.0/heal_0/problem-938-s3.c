#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int find_closest_triplet(const int *a, size_t a_size,
                                const int *b, size_t b_size,
                                const int *c, size_t c_size,
                                int *a_value, int *b_value, int *c_value)
{
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    unsigned long long best_range = ULLONG_MAX;

    if (a == NULL || b == NULL || c == NULL ||
        a_value == NULL || b_value == NULL || c_value == NULL ||
        a_size == 0 || b_size == 0 || c_size == 0) {
        return -1;
    }

    while (i < a_size && j < b_size && k < c_size) {
        int minimum = a[i];
        int maximum = a[i];

        if (b[j] < minimum) {
            minimum = b[j];
        }
        if (c[k] < minimum) {
            minimum = c[k];
        }
        if (b[j] > maximum) {
            maximum = b[j];
        }
        if (c[k] > maximum) {
            maximum = c[k];
        }

        unsigned long long range =
            (unsigned long long)((long long)maximum - (long long)minimum);

        if (range < best_range) {
            best_range = range;
            *a_value = a[i];
            *b_value = b[j];
            *c_value = c[k];

            if (best_range == 0) {
                return 0;
            }
        }

        if (a[i] == minimum) {
            ++i;
        } else if (b[j] == minimum) {
            ++j;
        } else {
            ++k;
        }
    }

    return 0;
}

int main(void)
{
    const int a[] = {1, 4, 10};
    const int b[] = {2, 15, 20};
    const int c[] = {10, 12};
    int a_value;
    int b_value;
    int c_value;

    if (find_closest_triplet(a, sizeof(a) / sizeof(a[0]),
                             b, sizeof(b) / sizeof(b[0]),
                             c, sizeof(c) / sizeof(c[0]),
                             &a_value, &b_value, &c_value) != 0) {
        fputs("Failed to find a closest triplet.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%d %d %d\n", a_value, b_value, c_value) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}