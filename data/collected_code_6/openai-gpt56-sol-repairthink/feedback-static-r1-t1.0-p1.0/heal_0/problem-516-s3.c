#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t sortable_key(int value)
{
    return (uint32_t)value ^ ((uint32_t)1U << 31);
}

static int radix_sort(int *values, size_t count)
{
    int *buffer;
    int *source;
    int *destination;
    size_t byte_index;

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

    for (byte_index = 0U; byte_index < sizeof(uint32_t); ++byte_index) {
        size_t frequencies[256] = {0U};
        size_t positions[256];
        size_t i;
        unsigned int shift = (unsigned int)(byte_index * 8U);

        for (i = 0U; i < count; ++i) {
            unsigned int digit = (sortable_key(source[i]) >> shift) & 0xFFU;
            ++frequencies[digit];
        }

        positions[0] = 0U;
        for (i = 1U; i < 256U; ++i) {
            positions[i] = positions[i - 1U] + frequencies[i - 1U];
        }

        for (i = 0U; i < count; ++i) {
            unsigned int digit = (sortable_key(source[i]) >> shift) & 0xFFU;
            destination[positions[digit]++] = source[i];
        }

        {
            int *temporary = source;
            source = destination;
            destination = temporary;
        }
    }

    if (source != values) {
        for (size_t i = 0U; i < count; ++i) {
            values[i] = source[i];
        }
    }

    free(buffer);
    return 0;
}

int main(void)
{
    int values[] = {170, 45, 75, -90, 802, 24, 2, 66, -1, 0};
    size_t count = sizeof(values) / sizeof(values[0]);
    int result = radix_sort(values, count);

    if (result != 0) {
        fprintf(stderr, "radix_sort failed: %d\n", result);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < count; ++i) {
        if (printf("%d%s", values[i], i + 1U == count ? "\n" : " ") < 0) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}