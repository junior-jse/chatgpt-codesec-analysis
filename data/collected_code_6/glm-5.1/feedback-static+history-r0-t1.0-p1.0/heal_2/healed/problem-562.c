#include <stdio.h>
#include <stdlib.h>

int max_sublist_length(int * const *lists, const int *lengths, int count) {
    if (lists == NULL || lengths == NULL || count <= 0) {
        return 0;
    }

    int max_len = 0;
    for (int i = 0; i < count; i++) {
        if (lengths[i] > max_len) {
            max_len = lengths[i];
        }
    }

    return max_len;
}

int main() {
    int list1[] = {1, 2, 3};
    int list2[] = {4, 5, 6, 7, 8};
    int list3[] = {9, 10};
    
    int *lists[] = {list1, list2, list3};
    const int lengths[] = {3, 5, 2};
    int count = 3;

    int result = max_sublist_length(lists, lengths, count);
    printf("%d\n", result);

    return 0;
}