#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
} Tuple;

int remove_matching(Tuple *t1, Tuple *t2) {
    if (t1 == NULL || t2 == NULL || t1->data == NULL || t2->data == NULL) {
        return -1;
    }
    
    size_t new_size1 = 0;
    size_t new_size2 = 0;
    
    for (size_t i = 0; i < t1->size; i++) {
        int found = 0;
        for (size_t j = 0; j < t2->size; j++) {
            if (t1->data[i] == t2->data[j]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            t1->data[new_size1++] = t1->data[i];
        }
    }
    
    for (size_t i = 0; i < t2->size; i++) {
        int found = 0;
        for (size_t j = 0; j < t1->size; j++) {
            if (t2->data[i] == t1->data[j]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            t2->data[new_size2++] = t2->data[i];
        }
    }
    
    t1->size = new_size1;
    t2->size = new_size2;
    
    return 0;
}

int main(void) {
    int data1[] = {1, 2, 3, 4, 5};
    int data2[] = {2, 4, 6, 8};
    
    Tuple t1 = {data1, 5};
    Tuple t2 = {data2, 4};
    
    printf("Tuple 1 before: ");
    for (size_t i = 0; i < t1.size; i++) {
        printf("%d ", t1.data[i]);
    }
    printf("\n");
    
    printf("Tuple 2 before: ");
    for (size_t i = 0; i < t2.size; i++) {
        printf("%d ", t2.data[i]);
    }
    printf("\n");
    
    if (remove_matching(&t1, &t2) == 0) {
        printf("Tuple 1 after: ");
        for (size_t i = 0; i < t1.size; i++) {
            printf("%d ", t1.data[i]);
        }
        printf("\n");
        
        printf("Tuple 2 after: ");
        for (size_t i = 0; i < t2.size; i++) {
            printf("%d ", t2.data[i]);
        }
        printf("\n");
    } else {
        fprintf(stderr, "Error processing tuples\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}