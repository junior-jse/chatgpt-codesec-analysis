#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    int size;
} List;

typedef struct {
    List *lists;
    int count;
} ListOfLists;

int compare_lists(const void *a, const void *b) {
    const List *la = (const List *)a;
    const List *lb = (const List *)b;
    
    if (la->size == 0 || lb->size == 0) {
        return 0;
    }
    
    if (la->data[0] < lb->data[0]) return -1;
    if (la->data[0] > lb->data[0]) return 1;
    return 0;
}

void sort_list_of_lists(ListOfLists *lol, int index) {
    if (lol == NULL || lol->count == 0 || lol->lists == NULL) {
        return;
    }
    
    for (int i = 0; i < lol->count; i++) {
        if (lol->lists[i].size == 0 || lol->lists[i].data == NULL) {
            return;
        }
        if (index < 0 || index >= lol->lists[i].size) {
            return;
        }
    }
    
    for (int i = 0; i < lol->count; i++) {
        int temp = lol->lists[i].data[0];
        lol->lists[i].data[0] = lol->lists[i].data[index];
        lol->lists[i].data[index] = temp;
    }
    
    qsort(lol->lists, lol->count, sizeof(List), compare_lists);
    
    for (int i = 0; i < lol->count; i++) {
        int temp = lol->lists[i].data[0];
        lol->lists[i].data[0] = lol->lists[i].data[index];
        lol->lists[i].data[index] = temp;
    }
}

int main(void) {
    List lists[4];
    int d0[] = {3, 1, 4};
    int d1[] = {1, 5, 9};
    int d2[] = {2, 6, 5};
    int d3[] = {4, 3, 6};
    
    lists[0].data = d0; lists[0].size = 3;
    lists[1].data = d1; lists[1].size = 3;
    lists[2].data = d2; lists[2].size = 3;
    lists[3].data = d3; lists[3].size = 3;
    
    ListOfLists lol;
    lol.lists = lists;
    lol.count = 4;
    
    sort_list_of_lists(&lol, 1);
    
    for (int i = 0; i < lol.count; i++) {
        for (int j = 0; j < lol.lists[i].size; j++) {
            printf("%d ", lol.lists[i].data[j]);
        }
        printf("\n");
    }
    
    return 0;
}