#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>

static int max_profit(const int *prices, size_t count, size_t k, int64_t *result)
{
    if (result == NULL || (prices == NULL && count != 0U)) {
        return -1;
    }

    *result = 0;

    if (count < 2U || k == 0U) {
        return 0;
    }

    if (k >= count / 2U) {
        int64_t profit = 0;

        for (size_t i = 1U; i < count; ++i) {
            if (prices[i] > prices[i - 1U]) {
                profit += (int64_t)prices[i] - (int64_t)prices[i - 1U];
            }
        }

        *result = profit;
        return 0;
    }

    if (k > SIZE_MAX / sizeof(int64_t) - 1U) {
        return -1;
    }

    int64_t *profits = calloc(k + 1U, sizeof(*profits));
    int64_t *balances = malloc((k + 1U) * sizeof(*balances));

    if (profits == NULL || balances == NULL) {
        free(profits);
        free(balances);
        return -1;
    }

    balances[0] = 0;
    for (size_t transaction = 1U; transaction <= k; ++transaction) {
        balances[transaction] = -(int64_t)prices[0];
    }

    for (size_t day = 1U; day < count; ++day) {
        for (size_t transaction = 1U; transaction <= k; ++transaction) {
            int64_t buy = profits[transaction - 1U] - (int64_t)prices[day];
            if (buy > balances[transaction]) {
                balances[transaction] = buy;
            }

            int64_t sell = balances[transaction] + (int64_t)prices[day];
            if (sell > profits[transaction]) {
                profits[transaction] = sell;
            }
        }
    }

    *result = profits[k];

    free(balances);
    free(profits);
    return 0;
}

int main(void)
{
    size_t count;
    size_t k;

    if (scanf("%zu %zu", &count, &k) != 2) {
        fputs("Invalid input\n", stderr);
        return EXIT_FAILURE;
    }

    if (count > SIZE_MAX / sizeof(int)) {
        fputs("Input is too large\n", stderr);
        return EXIT_FAILURE;
    }

    int *prices = NULL;

    if (count != 0U) {
        prices = malloc(count * sizeof(*prices));
        if (prices == NULL) {
            fputs("Memory allocation failed\n", stderr);
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0U; i < count; ++i) {
        if (scanf("%d", &prices[i]) != 1 || prices[i] < 0) {
            fputs("Invalid price\n", stderr);
            free(prices);
            return EXIT_FAILURE;
        }
    }

    int64_t profit;
    if (max_profit(prices, count, k, &profit) != 0) {
        fputs("Unable to calculate profit\n", stderr);
        free(prices);
        return EXIT_FAILURE;
    }

    printf("%" PRId64 "\n", profit);

    free(prices);
    return EXIT_SUCCESS;
}