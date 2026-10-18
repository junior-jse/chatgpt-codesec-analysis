#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

int maxProfit(int k, const int *prices, int n) {
    if (prices == NULL || n <= 1 || k <= 0) {
        return 0;
    }

    if (k >= n / 2) {
        int profit = 0;
        for (int i = 1; i < n; i++) {
            if (prices[i] > prices[i - 1]) {
                profit += prices[i] - prices[i - 1];
            }
        }
        return profit;
    }

    int *buy = (int *)malloc((size_t)k * sizeof(int));
    int *sell = (int *)malloc((size_t)k * sizeof(int));
    
    if (buy == NULL || sell == NULL) {
        free(buy);
        free(sell);
        return 0;
    }

    for (int i = 0; i < k; i++) {
        buy[i] = INT_MIN;
        sell[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = k - 1; j >= 0; j--) {
            if (buy[j] != INT_MIN) {
                int candidate = buy[j] + prices[i];
                if (candidate > sell[j]) {
                    sell[j] = candidate;
                }
            }
            
            if (j > 0) {
                int candidate = sell[j - 1] - prices[i];
                if (candidate > buy[j]) {
                    buy[j] = candidate;
                }
            } else {
                int candidate = -prices[i];
                if (candidate > buy[j]) {
                    buy[j] = candidate;
                }
            }
        }
    }

    int result = sell[k - 1];
    free(buy);
    free(sell);
    
    return result;
}

int main(void) {
    int prices1[] = {2, 4, 1};
    int n1 = (int)(sizeof(prices1) / sizeof(prices1[0]));
    printf("Max profit (k=2): %d\n", maxProfit(2, prices1, n1));

    int prices2[] = {3, 2, 6, 5, 0, 3};
    int n2 = (int)(sizeof(prices2) / sizeof(prices2[0]));
    printf("Max profit (k=2): %d\n", maxProfit(2, prices2, n2));

    int prices3[] = {1, 2, 3, 4, 5};
    int n3 = (int)(sizeof(prices3) / sizeof(prices3[0]));
    printf("Max profit (k=2): %d\n", maxProfit(2, prices3, n3));

    int prices4[] = {7, 6, 4, 3, 1};
    int n4 = (int)(sizeof(prices4) / sizeof(prices4[0]));
    printf("Max profit (k=2): %d\n", maxProfit(2, prices4, n4));

    printf("Max profit (k=0): %d\n", maxProfit(0, prices1, n1));
    printf("Max profit (NULL prices): %d\n", maxProfit(2, NULL, 5));

    return 0;
}