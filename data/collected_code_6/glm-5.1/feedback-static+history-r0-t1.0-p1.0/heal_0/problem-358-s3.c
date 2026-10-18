#include <stdio.h>
#include <stdlib.h>

int* modulo_division(const int* a, const int* b, size_t size, int** error) {
    *error = NULL;
    if (size == 0) {
        return NULL;
    }
    int* result = (int*)malloc(size * sizeof(int));
    if (!result) {
        *error = (int*)1;
        return NULL;
    }
    for (size_t i = 0; i < size; i++) {
        if (b[i] == 0) {
            free(result);
            *error = (int*)2;
            return NULL;
        }
        result[i] = a[i] % b[i];
    }
    return result;
}

int main(void) {
    int a[] = {10, 25, 30, 14};
    int b[] = {3, 4, 5, 2};
    size_t size = sizeof(a) / sizeof(a[0]);
    int* err = NULL;
    int* res = modulo_division(a, b, size, &err);
    if (err != NULL) {
        free(err);
        return 1;
    }
    if (res == NULL) {
        return 1;
    }
    for (size_t i = 0; i < size; i++) {
        printf("%d ", res[i]);
    }
    printf("\n");
    free(res);
    return 0;
}