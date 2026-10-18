#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Possible weaknesses found:
 *  Parameter 'arr' can be declared as pointer to const [constParameterPointer]
 */
void find_max_product_pair(int *arr, int size, int *pair) {
    if (arr == NULL || pair == NULL || size < 2) {
        if (pair != NULL && size < 2) {
            pair[0] = 0;
            pair[1] = 0;
        }
        return;
    }

    int max1 = INT_MIN;
    int max2 = INT_MIN;
    int min1 = INT_MAX;
    int min2 = INT_MAX;

    for (int i = 0; i < size; ++i) {
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

    if (max1 * max2 > min1 * min2) {
        pair[0] = max1;
        pair[1] = max2;
    } else {
        pair[0] = min1;
        pair[1] = min2;
    }
}

int main(void) {
    int arr[] = {1, -4, 3, -6, 7, 0};
    /* Possible weaknesses found:
     *  Assignment 'size=sizeof(arr)/sizeof(arr[0])', assigned value is 6
     */
    int size = sizeof(arr) / sizeof(arr[0]);
    int pair[2];

    find_max_product_pair(arr, size, pair);

    /* Possible weaknesses found:
     *  Condition 'size>=2' is always true [knownConditionTrueFalse]
     *  Condition 'size>=2' is always true
     */
    if (size >= 2) {
        printf("%d %d\n", pair[0], pair[1]);
    }

    return 0;
}