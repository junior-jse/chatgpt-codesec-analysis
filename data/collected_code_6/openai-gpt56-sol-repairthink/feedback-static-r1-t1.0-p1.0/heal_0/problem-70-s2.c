#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static bool all_tuples_equal_length(const size_t lengths[], size_t count)
{
    if (lengths == NULL || count == 0U) {
        return false;
    }

    for (size_t i = 1U; i < count; ++i) {
        if (lengths[i] != lengths[0]) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    const size_t tuple_lengths[] = {3U, 3U, 3U, 3U};
    const size_t tuple_count =
        sizeof(tuple_lengths) / sizeof(tuple_lengths[0]);

    if (all_tuples_equal_length(tuple_lengths, tuple_count)) {
        if (puts("All tuples have equal length.") == EOF) {
            return 1;
        }
    } else {
        if (puts("Not all tuples have equal length.") == EOF) {
            return 1;
        }
    }

    return 0;
}