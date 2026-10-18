#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int count;
    int value;
} RLEPair;

typedef struct {
    RLEPair *data;
    size_t size;
    size_t capacity;
} RLEList;

RLEList* rle_encode(const int *list, size_t length) {
    RLEList *rle = (RLEList*)malloc(sizeof(RLEList));
    if (!rle) return NULL;
    
    rle->capacity = length > 0 ? length : 1;
    rle->data = (RLEPair*)malloc(rle->capacity * sizeof(RLEPair));
    if (!rle->data) {
        free(rle);
        return NULL;
    }
    rle->size = 0;

    if (length == 0) return rle;

    int current = list[0];
    int count = 1;

    for (size_t i = 1; i <= length; i++) {
        if (i < length && list[i] == current) {
            count++;
        } else {
            if (count >= 2) {
                rle->data[rle->size].count = count;
                rle->data[rle->size].value = current;
                rle->size++;
            } else {
                rle->data[rle->size].count = 1;
                rle->data[rle->size].value = current;
                rle->size++;
                
                size_t j = i;
                while (j < length && list[j] != list[j-1]) {
                    rle->data[rle->size].count = 1;
                    rle->data[rle->size].value = list[j];
                    rle->size++;
                    j++;
                }
                i = j - 1; 
                if (j < length) {
                    current = list[j];
                    count = 1;
                    continue;
                }
            }
            if (i < length) {
                current = list[i];
                count = 1;
            }
        }
    }
    return rle;
}

void rle_free(RLEList *rle) {
    if (rle) {
        free(rle->data);
        free(rle);
    }
}

void print_rle(const RLEList *rle) {
    for (size_t i = 0; i < rle->size; i++) {
        printf("%d:%d ", rle->data[i].count, rle->data[i].value);
    }
    printf("\n");
}

int main() {
    int list1[] = {1, 1, 1, 2, 3, 3, 4, 5, 5, 5};
    size_t len1 = sizeof(list1) / sizeof(list1[0]);
    
    RLEList *rle1 = rle_encode(list1, len1);
    if (rle1) {
        print_rle(rle1);
        rle_free(rle1);
    }

    int list2[] = {1, 2, 3, 4, 5};
    size_t len2 = sizeof(list2) / sizeof(list2[0]);
    
    RLEList *rle2 = rle_encode(list2, len2);
    if (rle2) {
        print_rle(rle2);
        rle_free(rle2);
    }

    /* Possible weaknesses found:
     *  Variable 'list3' can be declared as const array [constVariable]
     */
    int list3[] = {};
    size_t len3 = 0;
    
    RLEList *rle3 = rle_encode(list3, len3);
    if (rle3) {
        print_rle(rle3);
        rle_free(rle3);
    }

    return 0;
}