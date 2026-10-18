#include <stdio.h>
#include <stdlib.h>

int* remove_negatives(const int* src, size_t src_len, size_t* out_len) {
    if (src == NULL || out_len == NULL) {
        exit(EXIT_FAILURE);
    }

    size_t count = 0;
    for (size_t i = 0; i < src_len; ++i) {
        if (src[i] >= 0) {
            ++count;
        }
    }

    int* result = (int*)malloc(count * sizeof(int));
    if (result == NULL) {
        exit(EXIT_FAILURE);
    }

    size_t j = 0;
    for (size_t i = 0; i < src_len; ++i) {
        if (src[i] >= 0) {
            result[j] = src[i];
            ++j;
        }
    }

    *out_len = count;
    return result;
}

void in_place_remove_negatives(int* arr, size_t len, size_t* out_len) {
    if (arr == NULL || out_len == NULL) {
        exit(EXIT_FAILURE);
    }

    size_t j = 0;
    for (size_t i = 0; i < len; ++i) {
        if (arr[i] >= 0) {
            arr[j] = arr[i];
            ++j;
        }
    }
    *out_len = j;
}

int main(void) {
    int arr1[] = {1, -2, 3, -4, 5, 0, -6};
    size_t arr1_len = sizeof(arr1) / sizeof(arr1[0]);
    size_t out_len1 = 0;

    int* filtered1 = remove_negatives(arr1, arr1_len, &out_len1);

    for (size_t i = 0; i < out_len1; ++i) {
        printf("%d ", filtered1[i]);
    }
    printf("\n");

    free(filtered1);

    int arr2[] = {-1, -2, 3, 4, -5, 0};
    size_t arr2_len = sizeof(arr2) / sizeof(arr2[0]);
    size_t out_len2 = arr2_len;

    in_place_remove_negatives(arr2, arr2_len, &out_len2);

    for (size_t i = 0; i < out_len2; ++i) {
        printf("%d ", arr2[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}