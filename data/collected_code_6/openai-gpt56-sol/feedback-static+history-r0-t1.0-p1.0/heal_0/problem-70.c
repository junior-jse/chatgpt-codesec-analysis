#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    const void *elements;
    size_t length;
} Tuple;

bool all_tuples_have_equal_length(const Tuple *tuples, size_t count)
{
    if (count == 0U) {
        return true;
    }

    if (tuples == NULL) {
        return false;
    }

    const size_t expected_length = tuples[0].length;

    for (size_t i = 1U; i < count; ++i) {
        if (tuples[i].length != expected_length) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    const int first[] = {1, 2, 3};
    const int second[] = {4, 5, 6};
    const int third[] = {7, 8, 9};

    const Tuple tuples[] = {
        {first, sizeof first / sizeof first[0]},
        {second, sizeof second / sizeof second[0]},
        {third, sizeof third / sizeof third[0]}
    };

    const size_t tuple_count = sizeof tuples / sizeof tuples[0];

    if (all_tuples_have_equal_length(tuples, tuple_count)) {
        if (puts("All tuples have equal length.") == EOF) {
            return 1;
        }
    } else {
        if (puts("The tuples do not have equal length.") == EOF) {
            return 1;
        }
    }

    return 0;
}