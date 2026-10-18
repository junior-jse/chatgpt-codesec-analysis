#include <stdio.h>
#include <stdlib.h>

int* get_first_elements(int** sublists, const int* lengths, int num_sublists, int* out_len) {
    if (sublists == NULL || lengths == NULL || out_len == NULL || num_sublists <= 0) {
        if (out_len) *out_len = 0;
        return NULL;
    }

    int* result = (int*)malloc(num_sublists * sizeof(int));
    if (result == NULL) {
        *out_len = 0;
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < num_sublists; i++) {
        if (sublists[i] != NULL && lengths[i] > 0) {
            result[count++] = sublists[i][0];
        }
    }

    *out_len = count;
    return result;
}

int main() {
    int a[] = {1, 2, 3};
    int b[] = {4, 5};
    int c[] = {6, 7, 8, 9};

    int* sublists[] = {a, b, NULL, c};
    /* Possible weaknesses found:
     *  Variable 'lengths' can be declared as const array [constVariable]
     */
    int lengths[] = {3, 2, 0, 4};
    int num_sublists = 4;

    int out_len = 0;
    int* result = get_first_elements(sublists, lengths, num_sublists, &out_len);

    if (result != NULL) {
        for (int i = 0; i < out_len; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    }

    return 0;
}