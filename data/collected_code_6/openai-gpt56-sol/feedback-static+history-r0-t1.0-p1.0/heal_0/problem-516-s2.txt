#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int radix_sort(int *values, size_t count)
{
    int *output;
    int *source = values;
    int *destination;
    size_t shift;

    if (count < 2) {
        return 0;
    }

    if (values == NULL || count > SIZE_MAX / sizeof(*output)) {
        errno = EINVAL;
        return -1;
    }

    output = malloc(count * sizeof(*output));
    if (output == NULL) {
        return -1;
    }

    destination = output;

    for (shift = 0; shift < sizeof(uint32_t) * 8U; shift += 8U) {
        size_t buckets[256] = {0};
        size_t offsets[256];
        size_t i;

        for (i = 0; i < count; ++i) {
            uint32_t key = (uint32_t)source[i] ^ UINT32_C(0x80000000);
            ++buckets[(key >> shift) & UINT32_C(0xff)];
        }

        offsets[0] = 0;
        for (i = 1; i < 256U; ++i) {
            offsets[i] = offsets[i - 1U] + buckets[i - 1U];
        }

        for (i = 0; i < count; ++i) {
            uint32_t key = (uint32_t)source[i] ^ UINT32_C(0x80000000);
            size_t bucket = (size_t)((key >> shift) & UINT32_C(0xff));
            destination[offsets[bucket]++] = source[i];
        }

        {
            int *temporary = source;
            source = destination;
            destination = temporary;
        }
    }

    if (source != values) {
        for (size_t i = 0; i < count; ++i) {
            values[i] = source[i];
        }
    }

    free(output);
    return 0;
}

int main(void)
{
    int values[] = {170, -45, 75, -90, 802, 24, 2, 66, 0};
    size_t count = sizeof(values) / sizeof(values[0]);

    if (radix_sort(values, count) != 0) {
        perror("radix_sort");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; ++i) {
        if (printf("%d%s", values[i], i + 1U == count ? "\n" : " ") < 0) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}