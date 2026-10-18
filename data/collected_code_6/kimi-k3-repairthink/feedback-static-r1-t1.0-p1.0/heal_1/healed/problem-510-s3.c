#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int compare_ull(const void *a, const void *b)
{
    unsigned long long x = *(const unsigned long long *)a;
    unsigned long long y = *(const unsigned long long *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

unsigned long long count_subsequences_product_less_than_k(const unsigned long long *arr, size_t n, unsigned long long k)
{
    if (arr == NULL || n == 0 || k == 0) {
        return 0;
    }

    unsigned long long *sorted = (unsigned long long *)malloc(n * sizeof(unsigned long long));
    if (sorted == NULL) {
        return 0;
    }

    for (size_t i = 0; i < n; i++) {
        sorted[i] = arr[i];
    }

    qsort(sorted, n, sizeof(unsigned long long), compare_ull);

    unsigned long long count = 0;
    size_t left = 0;
    size_t right = n - 1;

    while (left <= right) {
        if (sorted[left] == 0) {
            count += (1ULL << (right - left + 1)) - 1;
            break;
        }

        if (sorted[left] <= k / sorted[right] && sorted[left] * sorted[right] < k) {
            count += (1ULL << (right - left));
            left++;
        } else {
            if (right == 0) {
                break;
            }
            right--;
        }
    }

    free(sorted);
    return count;
}

int main(void)
{
    unsigned long long arr[] = {1, 2, 3, 4};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    unsigned long long k = 10;

    unsigned long long result = count_subsequences_product_less_than_k(arr, n, k);
    printf("%llu\n", result);

    return 0;
}