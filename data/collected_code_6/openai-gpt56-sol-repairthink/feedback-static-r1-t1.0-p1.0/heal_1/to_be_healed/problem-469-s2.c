#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>

static int64_t max_profit(const int64_t *prices, size_t count, size_t k)
{
    if (prices == NULL || count < 2U || k == 0U) {
        return 0;
    }

    if (k >= count / 2U) {
        int64_t profit = 0;

        for (size_t i = 1U; i < count; ++i) {
            if (prices[i] > prices[i - 1U]) {
                profit += prices[i] - prices[i - 1U];
            }
        }

        return profit;
    }

    if (k > SIZE_MAX / sizeof(int64_t) - 1U) {
        return 0;
    }

    int64_t *previous = calloc(k + 1U, sizeof(*previous));
    int64_t *current = calloc(k + 1U, sizeof(*current));

    if (previous == NULL || current == NULL) {
        free(previous);
        free(current);
        return 0;
    }

    for (size_t day = 1U; day < count; ++day) {
        int64_t best_difference = -prices[0];
        current[0] = 0;

        for (size_t transaction = 1U; transaction <= k; ++transaction) {
            int64_t sell_profit = prices[day] + best_difference;

            current[transaction] =
                previous[transaction] > sell_profit
                    ? previous[transaction]
                    : sell_profit;

            int64_t candidate =
                previous[transaction - 1U] - prices[day];

            if (candidate > best_difference) {
                best_difference = candidate;
            }
        }

        int64_t *temporary = previous;
        previous = current;
        current = temporary;
    }

    int64_t result = previous[k];

    free(previous);
    free(current);

    return result;
}

int main(void)
{
    size_t count = 0U;
    size_t k = 0U;

    if (scanf("%zu %zu", &count, &k) != 2 || count == 0U) {
        return EXIT_FAILURE;
    }

    if (count > SIZE_MAX / sizeof(int64_t)) {
        return EXIT_FAILURE;
    }

    int64_t *prices = malloc(count * sizeof(*prices));

    if (prices == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < count; ++i) {
        /* Possible weaknesses found:
         * Flawfinder scanf: The scanf() family's %s operation, without a limit specification, permits buffer overflows (CWE-120, CWE-20). Specify a limit to %s, or use a different input function. If the scanf format is influenceable by an attacker, it's exploitable. (risk 4, buffer)
         */
        if (scanf("%" SCNd64, &prices[i]) != 1 || prices[i] < 0) {
            free(prices);
            return EXIT_FAILURE;
        }
    }

    int64_t profit = max_profit(prices, count, k);
    printf("%" PRId64 "\n", profit);

    free(prices);
    return EXIT_SUCCESS;
}