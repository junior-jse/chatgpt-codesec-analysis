#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
 /* Possible weaknesses found:
  *  include '<string.h>' or provide a declaration of 'memmove'
  */

static uint64_t saturating_add(uint64_t a, uint64_t b)
{
    return UINT64_MAX - a < b ? UINT64_MAX : a + b;
}

static uint64_t saturating_mul2(uint64_t value)
{
    return value > UINT64_MAX / 2 ? UINT64_MAX : value * 2;
}

static uint64_t count_subsequences(const uint64_t *array, size_t length, uint64_t k)
{
    uint64_t *products;
    uint64_t count = 0;
    size_t used = 1;

    if (array == NULL || length == 0 || k == 0) {
        return 0;
    }

    if (length > (SIZE_MAX / sizeof(*products)) - 1) {
        return UINT64_MAX;
    }

    products = malloc((length + 1) * sizeof(*products));
    if (products == NULL) {
        return UINT64_MAX;
    }

    products[0] = 1;

    for (size_t index = 0; index < length; ++index) {
        uint64_t value = array[index];

        if (value == 0) {
            count = saturating_add(saturating_mul2(count), 1);
            continue;
        }

        if (value >= k) {
            continue;
        }

        uint64_t new_count = 0;

        for (size_t position = 0; position < used; ++position) {
            if (products[position] <= (k - 1) / value) {
                products[new_count++] = products[position] * value;
            }
        }

        count = saturating_add(count, new_count);

        if (new_count > 0) {
            /* Possible weaknesses found:
             *  call to undeclared library function 'memmove' with type 'void *(void *, const void *, unsigned long)'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
             *  implicit declaration of function 'memmove' [-Wimplicit-function-declaration]
             *  include the header <string.h> or explicitly provide a declaration for 'memmove'
             */
            memmove(products + new_count, products, used * sizeof(*products));
            used += new_count;
        }
    }

    free(products);
    return count;
}

int main(void)
{
    size_t length;
    uint64_t k;
    uint64_t *array = NULL;

    /* Possible weaknesses found:
     * Flawfinder scanf: The scanf() family's %s operation, without a limit specification, permits buffer overflows (CWE-120, CWE-20). Specify a limit to %s, or use a different input function. If the scanf format is influenceable by an attacker, it's exploitable. (risk 4, buffer)
     */
    if (scanf("%zu %" SCNu64, &length, &k) != 2) {
        return EXIT_FAILURE;
    }

    if (length > SIZE_MAX / sizeof(*array)) {
        return EXIT_FAILURE;
    }

    if (length > 0) {
        array = malloc(length * sizeof(*array));
        if (array == NULL) {
            return EXIT_FAILURE;
        }
    }

    for (size_t index = 0; index < length; ++index) {
        /* Possible weaknesses found:
         * Flawfinder scanf: The scanf() family's %s operation, without a limit specification, permits buffer overflows (CWE-120, CWE-20). Specify a limit to %s, or use a different input function. If the scanf format is influenceable by an attacker, it's exploitable. (risk 4, buffer)
         */
        if (scanf("%" SCNu64, &array[index]) != 1) {
            free(array);
            return EXIT_FAILURE;
        }
    }

    uint64_t result = count_subsequences(array, length, k);
    free(array);

    if (result == UINT64_MAX) {
        return EXIT_FAILURE;
    }

    printf("%" PRIu64 "\n", result);
    return EXIT_SUCCESS;
}