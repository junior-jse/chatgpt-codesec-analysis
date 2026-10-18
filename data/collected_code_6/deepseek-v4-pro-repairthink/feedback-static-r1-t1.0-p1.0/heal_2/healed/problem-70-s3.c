#include <stdio.h>
#include <stdbool.h>

typedef struct {
    size_t length;
} Tuple;

bool all_tuples_equal_length(const Tuple *tuples, size_t tuple_count) {
    if (tuples == NULL) {
        return tuple_count == 0;
    }
    if (tuple_count == 0) {
        return true;
    }
    size_t first_length = tuples[0].length;
    for (size_t i = 1; i < tuple_count; i++) {
        if (tuples[i].length != first_length) {
            return false;
        }
    }
    return true;
}

int main(void) {
    const Tuple tuples1[] = {
        {3},
        {3},
        {3}
    };
    const Tuple tuples2[] = {
        {3},
        {3},
        {2}
    };

    struct TestCase {
        const Tuple *tuples;
        size_t count;
        bool expected;
    } tests[] = {
        {tuples1, 3, true},
        {tuples2, 3, false},
        {NULL, 0, true},
        {NULL, 1, false},
        {tuples1, 0, true}
    };

    size_t num_tests = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < num_tests; i++) {
        bool result = all_tuples_equal_length(tests[i].tuples, tests[i].count);
        printf("Test %zu: %s\n", i + 1, result == tests[i].expected ? "pass" : "fail");
    }

    return 0;
}