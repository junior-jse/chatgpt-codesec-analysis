#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    /* Possible weaknesses found:
     *  struct member 'Tuple::elements' is never used. [unusedStructMember]
     */
    const void *elements;
    size_t length;
} Tuple;

static bool all_tuples_have_equal_length(const Tuple *tuples, size_t count)
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
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
    };

    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    const bool equal = all_tuples_have_equal_length(tuples, tuple_count);

    /* Possible weaknesses found:
     * Flawfinder equal: Function does not check the second iterator for over-read conditions (CWE-126). This function is often discouraged by most C++ coding standards in favor of its safer alternatives provided since C++14. Consider using a form of this function that checks the second iterator before potentially overflowing it. (risk 1, buffer)
     */
    if (printf("%s\n", equal ? "All tuples have equal length."
                             : "Tuples do not have equal length.") < 0) {
        return 1;
    }

    return 0;
}