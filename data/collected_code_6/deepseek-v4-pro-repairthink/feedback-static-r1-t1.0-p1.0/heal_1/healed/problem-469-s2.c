#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int max_profit(int k, const int *prices, int prices_size) {
    if (prices == NULL || prices_size <= 1 || k <= 0) {
        return 0;
    }

    if (k >= prices_size / 2) {
        int profit = 0;
        for (int i = 1; i < prices_size; i++) {
            if (prices[i] > prices[i - 1]) {
                profit += prices[i] - prices[i - 1];
            }
        }
        return profit;
    }

    int *buy = (int *)malloc((k + 1) * sizeof(int));
    int *sell = (int *)malloc((k + 1) * sizeof(int));

    if (buy == NULL || sell == NULL) {
        free(buy);
        free(sell);
        return 0;
    }

    for (int i = 0; i <= k; i++) {
        buy[i] = INT_MIN;
        sell[i] = 0;
    }

    for (int i = 0; i < prices_size; i++) {
        for (int j = 1; j <= k; j++) {
            int prev_buy = buy[j];
            buy[j] = (sell[j - 1] - prices[i] > buy[j]) ? sell[j - 1] - prices[i] : buy[j];
            sell[j] = (prev_buy + prices[i] > sell[j]) ? prev_buy + prices[i] : sell[j];
        }
    }

    int result = sell[k];
    free(buy);
    free(sell);
    return result;
}

int main(void) {
    int prices1[] = {2, 4, 1};
    int k1 = 2;
    int size1 = sizeof(prices1) / sizeof(prices1[0]);
    printf("Max profit (prices1, k=2): %d\n", max_profit(k1, prices1, size1));

    int prices2[] = {3, 2, 6, 5, 0, 3};
    int k2 = 2;
    int size2 = sizeof(prices2) / sizeof(prices2[0]);
    printf("Max profit (prices2, k=2): %d\n", max_profit(k2, prices2, size2));

    int prices3[] = {1, 2, 3, 4, 5};
    int k3 = 2;
    int size3 = sizeof(prices3) / sizeof(prices3[0]);
    printf("Max profit (prices3, k=2): %d\n", max_profit(k3, prices3, size3));

    int prices4[] = {5, 4, 3, 2, 1};
    int k4 = 2;
    int size4 = sizeof(prices4) / sizeof(prices4[0]);
    printf("Max profit (prices4, k=2): %d\n", max_profit(k4, prices4, size4));

    int prices5[] = {3, 3, 5, 0, 0, 3, 1, 4};
    int k5 = 2;
    int size5 = sizeof(prices5) / sizeof(prices5[0]);
    printf("Max profit (prices5, k=2): %d\n", max_profit(k5, prices5, size5));

    return 0;
}