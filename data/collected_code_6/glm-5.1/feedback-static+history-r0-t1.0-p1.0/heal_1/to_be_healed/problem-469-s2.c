#include <stdio.h>
#include <stdlib.h>

/* Possible weaknesses found:
 *  Parameter 'prices' can be declared as pointer to const [constParameterPointer]
 */
int max_profit(int k, int *prices, int prices_size) {
    if (prices_size <= 1 || k <= 0) {
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

    int *buy = (int *)malloc(k * sizeof(int));
    int *sell = (int *)malloc(k * sizeof(int));
    
    if (buy == NULL || sell == NULL) {
        free(buy);
        free(sell);
        return 0;
    }

    for (int i = 0; i < k; i++) {
        buy[i] = -prices[0];
        sell[i] = 0;
    }

    for (int i = 1; i < prices_size; i++) {
        buy[0] = (buy[0] > -prices[i]) ? buy[0] : -prices[i];
        sell[0] = (sell[0] > buy[0] + prices[i]) ? sell[0] : buy[0] + prices[i];
        
        for (int j = 1; j < k; j++) {
            buy[j] = (buy[j] > sell[j - 1] - prices[i]) ? buy[j] : sell[j - 1] - prices[i];
            sell[j] = (sell[j] > buy[j] + prices[i]) ? sell[j] : buy[j] + prices[i];
        }
    }

    int result = sell[k - 1];
    free(buy);
    free(sell);
    
    return result;
}

int main() {
    int prices1[] = {2, 4, 1};
    int k1 = 2;
    printf("%d\n", max_profit(k1, prices1, sizeof(prices1) / sizeof(prices1[0])));

    int prices2[] = {3, 2, 6, 5, 0, 3};
    int k2 = 2;
    printf("%d\n", max_profit(k2, prices2, sizeof(prices2) / sizeof(prices2[0])));

    return 0;
}