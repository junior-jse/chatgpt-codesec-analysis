#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    int first;
    int second;
} Tuple;

static bool has_all_k_elements(const Tuple *tuples, size_t count, size_t k)
{
    if (k == 0) {
        return true;
    }

    if (tuples == NULL || count == 0) {
        return false;
    }

    bool found[k];

    for (size_t i = 0; i < k; ++i) {
        found[i] = false;
    }

    for (size_t i = 0; i < count; ++i) {
        if (tuples[i].first >= 0 && (size_t)tuples[i].first < k) {
            found[(size_t)tuples[i].first] = true;
        }

        if (tuples[i].second >= 0 && (size_t)tuples[i].second < k) {
            found[(size_t)tuples[i].second] = true;
        }
    }

    for (size_t i = 0; i < k; ++i) {
        if (!found[i]) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    const Tuple tuples[] = {
        {0, 1},
        {2, 3},
        {1, 2}
    };
    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    const size_t k = 4;

    if (has_all_k_elements(tuples, tuple_count, k)) {
        puts("The tuple list contains all k elements.");
    } else {
        puts("The tuple list does not contain all k elements.");
    }

    return 0;
}