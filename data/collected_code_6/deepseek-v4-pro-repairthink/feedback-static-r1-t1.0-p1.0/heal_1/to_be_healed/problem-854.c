#include <stdio.h>
#include <stdlib.h>

typedef int (*compare_func)(const void *, const void *);

static void swap(void *a, void *b, size_t size) {
    unsigned char *pa = (unsigned char *)a;
    unsigned char *pb = (unsigned char *)b;
    /* Possible weaknesses found:
     *  The scope of the variable 'tmp' can be reduced. [variableScope]
     */
    unsigned char tmp;
    size_t i;
    for (i = 0; i < size; i++) {
        tmp = pa[i];
        pa[i] = pb[i];
        pb[i] = tmp;
    }
}

static void heapify(void *base, size_t nitems, size_t size, compare_func cmp, size_t i) {
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;
    unsigned char *arr = (unsigned char *)base;
    if (left < nitems && cmp(arr + left * size, arr + largest * size) > 0) {
        largest = left;
    }
    if (right < nitems && cmp(arr + right * size, arr + largest * size) > 0) {
        largest = right;
    }
    if (largest != i) {
        swap(arr + i * size, arr + largest * size, size);
        heapify(base, nitems, size, cmp, largest);
    }
}

void heapify_list(void *base, size_t nitems, size_t size, compare_func cmp) {
    if (base == NULL || size == 0 || nitems <= 1 || cmp == NULL) {
        return;
    }
    size_t i;
    for (i = nitems / 2; i > 0; i--) {
        heapify(base, nitems, size, cmp, i - 1);
    }
}

int int_cmp(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

int main(void) {
    int arr[] = {12, 11, 13, 5, 6, 7};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    size_t i;
    heapify_list(arr, n, sizeof(int), int_cmp);
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}