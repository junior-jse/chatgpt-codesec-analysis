#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>

static int max_profit(const int *prices, size_t count, size_t k, int64_t *result)
{
    int64_t *profits;
    size_t transaction;
    size_t day;

    if (result == NULL || (prices == NULL && count != 0U)) {
        return -1;
    }

    *result = 0;

    if (count < 2U || k == 0U) {
        return 0;
    }

    if (k >= count / 2U) {
        for (day = 1U; day < count; ++day) {
            if (prices[day] > prices[day - 1U]) {
                *result += (int64_t)prices[day] - (int64_t)prices[day - 1U];
            }
        }
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*profits)) {
        return -1;
    }

    profits = calloc(count, sizeof(*profits));
    if (profits == NULL) {
        return -1;
    }

    for (transaction = 1U; transaction <= k; ++transaction) {
        int64_t best_balance = -(int64_t)prices[0];
        int64_t previous_day_profit = profits[0];

        for (day = 1U; day < count; ++day) {
            int64_t old_profit = profits[day];
            int64_t sell_profit = best_balance + (int64_t)prices[day];
            int64_t buy_balance = previous_day_profit - (int64_t)prices[day];

            if (sell_profit > profits[day - 1U]) {
                profits[day] = sell_profit;
            } else {
                profits[day] = profits[day - 1U];
            }

            if (buy_balance > best_balance) {
                best_balance = buy_balance;
            }

            previous_day_profit = old_profit;
        }
    }

    *result = profits[count - 1U];
    free(profits);
    return 0;
}

int main(void)
{
    size_t count;
    size_t k;
    int *prices = NULL;
    int64_t profit;
    size_t index;
    int status = EXIT_FAILURE;

    if (scanf("%zu %zu", &count, &k) != 2) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    if (count > SIZE_MAX / sizeof(*prices)) {
        fprintf(stderr, "Input is too large\n");
        return EXIT_FAILURE;
    }

    if (count != 0U) {
        prices = malloc(count * sizeof(*prices));
        if (prices == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return EXIT_FAILURE;
        }
    }

    for (index = 0U; index < count; ++index) {
        if (scanf("%d", &prices[index]) != 1 || prices[index] < 0) {
            fprintf(stderr, "Invalid price\n");
            goto cleanup;
        }
    }

    if (max_profit(prices, count, k, &profit) != 0) {
        fprintf(stderr, "Unable to calculate profit\n");
        goto cleanup;
    }

    if (printf("%" PRId64 "\n", profit) < 0) {
        fprintf(stderr, "Output error\n");
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    free(prices);
    return status;
}