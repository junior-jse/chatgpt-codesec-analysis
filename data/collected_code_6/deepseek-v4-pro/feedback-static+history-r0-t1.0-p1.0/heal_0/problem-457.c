#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int end;
} Sublist;

Sublist find_minimum_sublist(int *arr, int n, int *sublist_arr, int m) {
    Sublist result = {-1, -1};
    int min_length = n + 1;
    int *freq = (int *)calloc(m, sizeof(int));
    if (freq == NULL) {
        return result;
    }

    int required = 0;
    int *target_freq = (int *)calloc(m, sizeof(int));
    if (target_freq == NULL) {
        free(freq);
        return result;
    }

    for (int i = 0; i < m; i++) {
        if (target_freq[sublist_arr[i]] == 0) {
            required++;
        }
        target_freq[sublist_arr[i]]++;
    }

    int left = 0;
    int formed = 0;

    for (int right = 0; right < n; right++) {
        if (arr[right] < 0 || arr[right] >= m) {
            continue;
        }

        freq[arr[right]]++;
        if (target_freq[arr[right]] > 0 && freq[arr[right]] == target_freq[arr[right]]) {
            formed++;
        }

        while (left <= right && formed == required) {
            if (right - left + 1 < min_length) {
                min_length = right - left + 1;
                result.start = left;
                result.end = right;
            }

            if (arr[left] >= 0 && arr[left] < m) {
                freq[arr[left]]--;
                if (target_freq[arr[left]] > 0 && freq[arr[left]] < target_freq[arr[left]]) {
                    formed--;
                }
            }
            left++;
        }
    }

    free(freq);
    free(target_freq);
    return result;
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 2, 3, 1, 4, 2, 3};
    int sublist_arr[] = {2, 3};
    int n = sizeof(arr) / sizeof(arr[0]);
    int m = sizeof(sublist_arr) / sizeof(sublist_arr[0]);

    Sublist result = find_minimum_sublist(arr, n, sublist_arr, m);

    if (result.start != -1) {
        printf("Minimum sublist found from index %d to %d\n", result.start, result.end);
        printf("Elements: ");
        for (int i = result.start; i <= result.end; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    } else {
        printf("No valid sublist found\n");
    }

    return 0;
}