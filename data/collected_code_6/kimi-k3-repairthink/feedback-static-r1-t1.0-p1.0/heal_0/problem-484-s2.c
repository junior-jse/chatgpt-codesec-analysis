#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
} Tuple;

int tuples_equal(const Tuple *t1, const Tuple *t2) {
    if (t1 == NULL || t2 == NULL) {
        return 0;
    }
    if (t1->size != t2->size) {
        return 0;
    }
    if (t1->size == 0) {
        return 1;
    }
    if (t1->data == NULL || t2->data == NULL) {
        return 0;
    }
    return memcmp(t1->data, t2->data, t1->size * sizeof(int)) == 0;
}

int remove_matching_tuples(Tuple *t1, Tuple *t2) {
    if (t1 == NULL || t2 == NULL) {
        return -1;
    }
    if (t1->size > 0 && t1->data == NULL) {
        return -1;
    }
    if (t2->size > 0 && t2->data == NULL) {
        return -1;
    }
    
    if (tuples_equal(t1, t2)) {
        free(t1->data);
        free(t2->data);
        t1->data = NULL;
        t2->data = NULL;
        t1->size = 0;
        t2->size = 0;
        return 1;
    }
    return 0;
}

void print_tuple(const Tuple *t) {
    if (t == NULL || t->data == NULL || t->size == 0) {
        printf("()\n");
        return;
    }
    printf("(");
    for (size_t i = 0; i < t->size; i++) {
        printf("%d", t->data[i]);
        if (i < t->size - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}

int main(void) {
    Tuple t1 = {0};
    Tuple t2 = {0};
    Tuple t3 = {0};
    Tuple t4 = {0};
    
    t1.size = 3;
    t1.data = malloc(t1.size * sizeof(int));
    if (t1.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    t1.data[0] = 1;
    t1.data[1] = 2;
    t1.data[2] = 3;
    
    t2.size = 3;
    t2.data = malloc(t2.size * sizeof(int));
    if (t2.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(t1.data);
        return 1;
    }
    t2.data[0] = 1;
    t2.data[1] = 2;
    t2.data[2] = 3;
    
    t3.size = 2;
    t3.data = malloc(t3.size * sizeof(int));
    if (t3.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(t1.data);
        free(t2.data);
        return 1;
    }
    t3.data[0] = 4;
    t3.data[1] = 5;
    
    t4.size = 2;
    t4.data = malloc(t4.size * sizeof(int));
    if (t4.data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(t1.data);
        free(t2.data);
        free(t3.data);
        return 1;
    }
    t4.data[0] = 6;
    t4.data[1] = 7;
    
    printf("Tuple 1: ");
    print_tuple(&t1);
    printf("Tuple 2: ");
    print_tuple(&t2);
    
    int result = remove_matching_tuples(&t1, &t2);
    if (result == 1) {
        printf("Tuples matched and were removed.\n");
    } else if (result == 0) {
        printf("Tuples did not match.\n");
    } else {
        printf("Error occurred.\n");
    }
    
    printf("Tuple 1 after: ");
    print_tuple(&t1);
    printf("Tuple 2 after: ");
    print_tuple(&t2);
    
    printf("\nTuple 3: ");
    print_tuple(&t3);
    printf("Tuple 4: ");
    print_tuple(&t4);
    
    result = remove_matching_tuples(&t3, &t4);
    if (result == 1) {
        printf("Tuples matched and were removed.\n");
    } else if (result == 0) {
        printf("Tuples did not match.\n");
    } else {
        printf("Error occurred.\n");
    }
    
    printf("Tuple 3 after: ");
    print_tuple(&t3);
    printf("Tuple 4 after: ");
    print_tuple(&t4);
    
    free(t1.data);
    free(t2.data);
    free(t3.data);
    free(t4.data);
    
    return 0;
}