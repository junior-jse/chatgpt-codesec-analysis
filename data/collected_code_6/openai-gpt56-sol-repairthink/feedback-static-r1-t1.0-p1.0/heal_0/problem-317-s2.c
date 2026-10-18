#include <stdio.h>
#include <stdlib.h>

typedef enum {
    RLE_SINGLE,
    RLE_MULTIPLE
} RLEKind;

typedef struct {
    RLEKind kind;
    size_t count;
    int value;
} RLEItem;

static int modified_run_length_encode(const int *input, size_t length,
                                      RLEItem **output, size_t *output_length)
{
    RLEItem *encoded;
    size_t groups = 0;
    size_t i = 0;
    size_t j = 0;

    if (output == NULL || output_length == NULL ||
        (input == NULL && length != 0)) {
        return -1;
    }

    *output = NULL;
    *output_length = 0;

    if (length == 0) {
        return 0;
    }

    groups = 1;
    for (i = 1; i < length; ++i) {
        if (input[i] != input[i - 1]) {
            ++groups;
        }
    }

    if (groups > SIZE_MAX / sizeof(*encoded)) {
        return -1;
    }

    encoded = malloc(groups * sizeof(*encoded));
    if (encoded == NULL) {
        return -1;
    }

    i = 0;
    while (i < length) {
        size_t run_length = 1;

        while (run_length < length - i &&
               input[i + run_length] == input[i]) {
            ++run_length;
        }

        encoded[j].kind = run_length == 1 ? RLE_SINGLE : RLE_MULTIPLE;
        encoded[j].count = run_length;
        encoded[j].value = input[i];

        ++j;
        i += run_length;
    }

    *output = encoded;
    *output_length = groups;
    return 0;
}

static void print_encoded(const RLEItem *encoded, size_t length)
{
    size_t i;

    putchar('[');

    for (i = 0; i < length; ++i) {
        if (i != 0) {
            printf(", ");
        }

        if (encoded[i].kind == RLE_SINGLE) {
            printf("%d", encoded[i].value);
        } else {
            printf("(%zu, %d)", encoded[i].count, encoded[i].value);
        }
    }

    puts("]");
}

int main(void)
{
    const int input[] = {
        1, 1, 1, 2, 3, 3, 4, 5, 5, 5, 5, 6
    };
    const size_t input_length = sizeof(input) / sizeof(input[0]);
    RLEItem *encoded = NULL;
    size_t encoded_length = 0;

    if (modified_run_length_encode(input, input_length,
                                   &encoded, &encoded_length) != 0) {
        fputs("Encoding failed\n", stderr);
        return EXIT_FAILURE;
    }

    print_encoded(encoded, encoded_length);
    free(encoded);

    return EXIT_SUCCESS;
}