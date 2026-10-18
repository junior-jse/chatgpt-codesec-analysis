#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;

static ll *dp;
static ll *dp_new;
static int *values;
static int num_values;
static ll min_val, max_val;

ll count_sequences(int length) {
    if (length <= 0) return 0;
    if (num_values == 0) return 0;
    
    min_val = values[0];
    max_val = values[0];
    for (int i = 1; i < num_values; i++) {
        if (values[i] < min_val) min_val = values[i];
        if (values[i] > max_val) max_val = values[i];
    }
    
    if (min_val >= 0) {
        ll result = 1;
        for (int i = 0; i < length; i++) {
            result *= num_values;
        }
        return result;
    }
    
    ll max_reach = 0;
    ll min_reach = 0;
    for (int i = 0; i < length; i++) {
        if (max_val > 0) max_reach += max_val;
        if (min_val < 0) min_reach += min_val;
    }
    
    ll offset = -min_reach;
    ll size = max_reach - min_reach + 1;
    
    if (size <= 0 || size > 100000000) return -1;
    
    dp = (ll *)calloc((size_t)size, sizeof(ll));
    dp_new = (ll *)calloc((size_t)size, sizeof(ll));
    if (dp == NULL || dp_new == NULL) {
        free(dp);
        free(dp_new);
        dp = NULL;
        dp_new = NULL;
        return -1;
    }
    
    dp[offset] = 1;
    
    for (int pos = 0; pos < length; pos++) {
        memset(dp_new, 0, (size_t)size * sizeof(ll));
        
        for (ll sum = 0; sum <= max_reach; sum++) {
            ll idx = sum + offset;
            if (idx < 0 || idx >= size) continue;
            if (dp[idx] == 0) continue;
            
            for (int v = 0; v < num_values; v++) {
                ll new_sum = sum + values[v];
                if (new_sum < 0) continue;
                if (new_sum > max_reach) continue;
                
                ll new_idx = new_sum + offset;
                if (new_idx >= 0 && new_idx < size) {
                    dp_new[new_idx] += dp[idx];
                }
            }
        }
        
        ll *temp = dp;
        dp = dp_new;
        dp_new = temp;
    }
    
    ll total = 0;
    for (ll sum = 0; sum <= max_reach; sum++) {
        ll idx = sum + offset;
        if (idx >= 0 && idx < size) {
            total += dp[idx];
        }
    }
    
    free(dp);
    free(dp_new);
    dp = NULL;
    dp_new = NULL;
    
    return total;
}

int main(void) {
    int length;
    int n;
    
    if (scanf("%d", &length) != 1) return 1;
    if (scanf("%d", &n) != 1) return 1;
    
    if (n <= 0 || length <= 0) {
        printf("0\n");
        return 0;
    }
    
    values = (int *)malloc((size_t)n * sizeof(int));
    if (values == NULL) return 1;
    num_values = n;
    
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &values[i]) != 1) {
            free(values);
            values = NULL;
            return 1;
        }
    }
    
    ll result = count_sequences(length);
    printf("%lld\n", result);
    
    free(values);
    values = NULL;
    
    return 0;
}