#include <stdbool.h>
#include <stddef.h>

bool is_even(int number) {
    return number % 2 == 0;
}

int main(void) {
    int test_values[] = {-4, -3, -2, -1, 0, 1, 2, 3, 4};
    size_t count = sizeof(test_values) / sizeof(test_values[0]);

    for (size_t i = 0; i < count; ++i) {
        if (is_even(test_values[i])) {
            return 0;
        }
    }

    return 1;
}