#include <stdio.h>
#include <stdlib.h>

int* list_modulo(const int* a, size_t len_a, const int* b, size_t len_b, size_t* out_len) {
    if (a == NULL || b == NULL || out_len == NULL) {
        return NULL;
    }
    if (len_a != len_b) {
        return NULL;
    }
    
    int* result = (int*)malloc(len_a * sizeof(int));
    if (result == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < len_a; ++i) {
        if (b[i] == 0) {
            free(result);
            return NULL;
        }
        result[i] = a[i] % b[i];
    }
    
    *out_len = len_a;
    return result;
}

int main(void) {
    int a[] = {10, 20, 30, 40, 50};
    int b[] = {3, 4, 0, 7, 8};
    size_t len_a = sizeof(a) / sizeof(a[0]);
    size_t len_b = sizeof(b) / sizeof(b[0]);
    size_t out_len = 0;
    
    int* result = list_modulo(a, len_a, b, len_b, &out_len);
    
    if (result != NULL) {
        for (size_t i = 0; i < out_len; ++i) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    } else {
        printf("Division by zero or invalid input detected.\n");
    }
    
    return 0;
}