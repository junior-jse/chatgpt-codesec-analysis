#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int radix_sort(int32_t *values, size_t count)
{
    int32_t *buffer;
    int32_t *source;
    int32_t *destination;

    if (values == NULL && count != 0U) {
        return EINVAL;
    }

    if (count < 2U) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*buffer)) {
        return EOVERFLOW;
    }

    buffer = malloc(count * sizeof(*buffer));
    if (buffer == NULL) {
        return ENOMEM;
    }

    source = values;
    destination = buffer;

    for (unsigned int shift = 0U; shift < 32U; shift += 8U) {
        size_t counts[256] = {0U};

        for (size_t i = 0U; i < count; ++i) {
            uint32_t key = ((uint32_t)source[i]) ^ UINT32_C(0x80000000);
            ++counts[(key >> shift) & UINT32_C(0xFF)];
        }

        for (size_t i = 1U; i < 256U; ++i) {
            counts[i] += counts[i - 1U];
        }

        for (size_t i = count; i > 0U; --i) {
            uint32_t key = ((uint32_t)source[i - 1U]) ^ UINT32_C(0x80000000);
            size_t bucket = (size_t)((key >> shift) & UINT32_C(0xFF));

            destination[--counts[bucket]] = source[i - 1U];
        }

        {
            int32_t *temporary = source;
            source = destination;
            destination = temporary;
        }
    }

    free(buffer);
    return 0;
}

int main(void)
{
    int32_t values[] = {
        INT32_C(170), INT32_C(45), INT32_C(75), INT32_C(-90),
        INT32_C(802), INT32_C(24), INT32_C(2), INT32_C(66),
        INT32_MIN, INT32_MAX, INT32_C(0)
    };
    const size_t count = sizeof(values) / sizeof(values[0]);
    int result = radix_sort(values, count);

    if (result != 0) {
        if (result == ENOMEM) {
            fputs("Memory allocation failed.\n", stderr);
        } else if (result == EOVERFLOW) {
            fputs("Input size is too large.\n", stderr);
        } else {
            fputs("Invalid input.\n", stderr);
        }
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < count; ++i) {
        if (printf("%d%s", values[i], i + 1U == count ? "\n" : " ") < 0) {
            fputs("Output failed.\n", stderr);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}