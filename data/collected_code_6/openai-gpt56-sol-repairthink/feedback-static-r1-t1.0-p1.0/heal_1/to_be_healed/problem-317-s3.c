#include <stdio.h>
#include <stdlib.h>
 /* Possible weaknesses found:
  *  'INT_MAX' is defined in header '<limits.h>'; this is probably fixable by adding '#include <limits.h>'
  */

typedef enum {
    ITEM_SINGLE,
    ITEM_MULTIPLE
} ItemType;

typedef struct {
    ItemType type;
    int count;
    int value;
} EncodedItem;

int modified_run_length_encode(const int *values, size_t length,
                               EncodedItem **encoded, size_t *encoded_length)
{
    EncodedItem *result;
    size_t output_index;
    size_t input_index;

    if (encoded == NULL || encoded_length == NULL) {
        return -1;
    }

    *encoded = NULL;
    *encoded_length = 0U;

    if (length == 0U) {
        return 0;
    }

    if (values == NULL || length > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(length * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    output_index = 0U;
    input_index = 0U;

    while (input_index < length) {
        size_t run_end = input_index + 1U;

        while (run_end < length && values[run_end] == values[input_index]) {
            ++run_end;
        }

        if (run_end - input_index == 1U) {
            result[output_index].type = ITEM_SINGLE;
            result[output_index].count = 1;
        } else {
            size_t count = run_end - input_index;

            /* Possible weaknesses found:
             *  each undeclared identifier is reported only once for each function it appears in
             *  use of undeclared identifier 'INT_MAX'
             *  'INT_MAX' undeclared (first use in this function)
             */
            if (count > (size_t)INT_MAX) {
                free(result);
                return -1;
            }

            result[output_index].type = ITEM_MULTIPLE;
            result[output_index].count = (int)count;
        }

        result[output_index].value = values[input_index];
        ++output_index;
        input_index = run_end;
    }

    *encoded = result;
    *encoded_length = output_index;
    return 0;
}

int main(void)
{
    const int values[] = {1, 1, 2, 3, 3, 3, 4, 5, 5};
    const size_t length = sizeof(values) / sizeof(values[0]);
    EncodedItem *encoded = NULL;
    size_t encoded_length = 0U;
    size_t index;

    if (modified_run_length_encode(values, length, &encoded,
                                   &encoded_length) != 0) {
        fputs("Encoding failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (index = 0U; index < encoded_length; ++index) {
        int status;

        if (encoded[index].type == ITEM_SINGLE) {
            status = printf("%d\n", encoded[index].value);
        } else {
            status = printf("(%d, %d)\n", encoded[index].count,
                            encoded[index].value);
        }

        if (status < 0) {
            free(encoded);
            return EXIT_FAILURE;
        }
    }

    free(encoded);
    return EXIT_SUCCESS;
}