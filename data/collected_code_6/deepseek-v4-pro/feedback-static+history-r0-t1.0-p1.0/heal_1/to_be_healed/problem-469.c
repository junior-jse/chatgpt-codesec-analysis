#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Possible weaknesses found:
 *  Parameter 'prices' can be declared as pointer to const [constParameterPointer]
 */
int maxProfit(int k, int *prices, int pricesSize) {
    if (prices == NULL || pricesSize < 2 || k <= 0) {
        return 0;
    }

    if (k >= pricesSize / 2) {
        int profit = 0;
        for (int i = 1; i < pricesSize; i++) {
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

    for (int i = 0; i < pricesSize; i++) {
        for (int j = 1; j <= k; j++) {
            buy[j] = buy[j] > (sell[j - 1] - prices[i]) ? buy[j] : (sell[j - 1] - prices[i]);
            sell[j] = sell[j] > (buy[j] + prices[i]) ? sell[j] : (buy[j] + prices[i]);
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
    printf("%d\n", maxProfit(k1, prices1, size1));

    int prices2[] = {3, 2, 6, 5, 0, 3};
    int k2 = 2;
    int size2 = sizeof(prices2) / sizeof(prices2[0]);
    printf("%d\n", maxProfit(k2, prices2, size2));

    int prices3[] = {1, 2, 3, 4, 5};
    int k3 = 2;
    int size3 = sizeof(prices3) / sizeof(prices3[0]);
    printf("%d\n", maxProfit(k3, prices3, size3));

    int *prices4 = NULL;
    int k4 = 2;
    printf("%d\n", maxProfit(k4, prices4, 0));

    int prices5[] = {5, 4, 3, 2, 1};
    int k5 = 2;
    int size5 = sizeof(prices5) / sizeof(prices5[0]);
    printf("%d\n", maxProfit(k5, prices5, size5));

    return 0;
}