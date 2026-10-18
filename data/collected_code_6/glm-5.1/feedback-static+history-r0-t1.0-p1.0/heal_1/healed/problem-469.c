#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
    return a > b ? a : b;
}

int maxProfit(int k, const int* prices, int pricesSize) {
    if (pricesSize <= 1 || k <= 0) {
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
    
    int* buy = (int*)malloc(k * sizeof(int));
    int* sell = (int*)malloc(k * sizeof(int));
    
    if (!buy || !sell) {
        free(buy);
        free(sell);
        return 0;
    }
    
    for (int i = 0; i < k; i++) {
        buy[i] = -prices[0];
        sell[i] = 0;
    }
    
    for (int i = 1; i < pricesSize; i++) {
        buy[0] = max(buy[0], -prices[i]);
        sell[0] = max(sell[0], buy[0] + prices[i]);
        for (int j = 1; j < k; j++) {
            buy[j] = max(buy[j], sell[j - 1] - prices[i]);
            sell[j] = max(sell[j], buy[j] + prices[i]);
        }
    }
    
    int result = sell[k - 1];
    
    free(buy);
    free(sell);
    
    return result;
}

int main() {
    int prices1[] = {3, 2, 6, 5, 0, 3};
    int k1 = 2;
    int size1 = sizeof(prices1) / sizeof(prices1[0]);
    printf("%d\n", maxProfit(k1, prices1, size1));
    
    int prices2[] = {1, 2, 3, 4, 5};
    int k2 = 1;
    int size2 = sizeof(prices2) / sizeof(prices2[0]);
    printf("%d\n", maxProfit(k2, prices2, size2));
    
    return 0;
}