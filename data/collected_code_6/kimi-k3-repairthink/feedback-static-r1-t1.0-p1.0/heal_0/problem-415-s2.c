#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

bool find_max_product_pair(const int arr[], size_t size, int *first, int *second, long long *product)
{
    size_t i;
    int max1, max2, min1, min2;
    long long prod_max, prod_min;
    
    if (arr == NULL || first == NULL || second == NULL || product == NULL || size < 2) {
        return false;
    }
    
    max1 = INT_MIN;
    max2 = INT_MIN;
    min1 = INT_MAX;
    min2 = INT_MAX;
    
    for (i = 0; i < size; i++) {
        if (arr[i] > max1) {
            max2 = max1;
            max1 = arr[i];
        } else if (arr[i] > max2) {
            max2 = arr[i];
        }
        
        if (arr[i] < min1) {
            min2 = min1;
            min1 = arr[i];
        } else if (arr[i] < min2) {
            min2 = arr[i];
        }
    }
    
    prod_max = (long long)max1 * (long long)max2;
    prod_min = (long long)min1 * (long long)min2;
    
    if (prod_max >= prod_min) {
        *first = max1;
        *second = max2;
        *product = prod_max;
    } else {
        *first = min1;
        *second = min2;
        *product = prod_min;
    }
    
    return true;
}

int main(void)
{
    int arr[] = {1, 4, 3, 6, 7, 0, -5, -8};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int first, second;
    long long product;
    
    if (find_max_product_pair(arr, size, &first, &second, &product)) {
        printf("Pair: (%d, %d)\n", first, second);
        printf("Product: %lld\n", product);
    } else {
        fprintf(stderr, "Error: Invalid input parameters\n");
        return 1;
    }
    
    return 0;
}