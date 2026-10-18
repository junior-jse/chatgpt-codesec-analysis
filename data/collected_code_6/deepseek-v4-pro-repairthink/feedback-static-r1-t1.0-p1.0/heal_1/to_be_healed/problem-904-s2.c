#include <stdbool.h>
 /* Possible weaknesses found:
  *  'size_t' is defined in header '<stddef.h>'; this is probably fixable by adding '#include <stddef.h>'
  */

bool is_even(int number) {
    return number % 2 == 0;
}

int main(void) {
    int test_values[] = {-4, -3, -2, -1, 0, 1, 2, 3, 4};
    /* Possible weaknesses found:
     *  use of undeclared identifier 'size_t'; did you mean 'sizeof'?
     *  unknown type name 'size_t'
     *  use of undeclared identifier 'count'
     */
    size_t count = sizeof(test_values) / sizeof(test_values[0]);

    /* Possible weaknesses found:
     *  unknown type name 'size_t'
     *  use of undeclared identifier 'i'
     *  use of undeclared identifier 'count'
     *  use of undeclared identifier 'size_t'; did you mean 'sizeof'?
     *  'size_t' is defined in header '<stddef.h>'; this is probably fixable by adding '#include <stddef.h>'
     */
    for (size_t i = 0; i < count; ++i) {
        /* Possible weaknesses found:
         *  use of undeclared identifier 'i'
         */
        if (is_even(test_values[i])) {
            return 0;
        }
    }

    return 1;
}