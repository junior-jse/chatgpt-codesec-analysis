#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

static uint64_t add_saturating(uint64_t a, uint64_t b)
{
    return UINT64_MAX - a < b ? UINT64_MAX : a + b;
}

static uint64_t mul2_saturating(uint64_t value)
{
    return value > UINT64_MAX / 2U ? UINT64_MAX : value * 2U;
}

static uint64_t count_subsequences_product_less_than_k(const uint64_t *array,
                                                        size_t length,
                                                        uint64_t k)
{
    if (array == NULL || length == 0U || k == 0U) {
        return 0U;
    }

    uint64_t zero_count = 0U;
    uint64_t positive_count = 0U;
    uint64_t total_without_zeros = 1U;

    for (size_t i = 0U; i < length; ++i) {
        if (array[i] == 0U) {
            zero_count = add_saturating(zero_count, total_without_zeros);
        } else {
            positive_count = add_saturating(
                positive_count,
                array[i] < k ? total_without_zeros : 0U
            );
            total_without_zeros = mul2_saturating(total_without_zeros);
            zero_count = mul2_saturating(zero_count);
        }
    }

    return add_saturating(zero_count, positive_count);
}

int main(void)
{
    size_t length;
    uint64_t k;

    /* Possible weaknesses found:
     * Flawfinder scanf: The scanf() family's %s operation, without a limit specification, permits buffer overflows (CWE-120, CWE-20). Specify a limit to %s, or use a different input function. If the scanf format is influenceable by an attacker, it's exploitable. (risk 4, buffer)
     */
    if (scanf("%zu %" SCNu64, &length, &k) != 2) {
        return EXIT_FAILURE;
    }

    if (length > SIZE_MAX / sizeof(uint64_t)) {
        return EXIT_FAILURE;
    }

    uint64_t *array = NULL;

    if (length > 0U) {
        array = malloc(length * sizeof(*array));
        if (array == NULL) {
            return EXIT_FAILURE;
        }

        for (size_t i = 0U; i < length; ++i) {
            /* Possible weaknesses found:
             * Flawfinder scanf: The scanf() family's %s operation, without a limit specification, permits buffer overflows (CWE-120, CWE-20). Specify a limit to %s, or use a different input function. If the scanf format is influenceable by an attacker, it's exploitable. (risk 4, buffer)
             */
            if (scanf("%" SCNu64, &array[i]) != 1) {
                free(array);
                return EXIT_FAILURE;
            }
        }
    }

    uint64_t result = count_subsequences_product_less_than_k(array, length, k);
    printf("%" PRIu64 "\n", result);

    free(array);
    return EXIT_SUCCESS;
}