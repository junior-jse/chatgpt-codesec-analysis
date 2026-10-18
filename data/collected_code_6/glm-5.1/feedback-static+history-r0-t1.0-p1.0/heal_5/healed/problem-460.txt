#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct Sublist {
    Node *head;
    struct Sublist *next;
} Sublist;

int *get_first_elements(Sublist *list, int *out_size) {
    if (list == NULL || out_size == NULL) {
        if (out_size != NULL) {
            *out_size = 0;
        }
        return NULL;
    }

    int count = 0;
    Sublist *curr = list;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }

    int *result = (int *)malloc(count * sizeof(int));
    if (result == NULL) {
        *out_size = 0;
        return NULL;
    }

    curr = list;
    for (int i = 0; i < count; i++) {
        if (curr->head != NULL) {
            result[i] = curr->head->value;
        } else {
            result[i] = 0;
        }
        curr = curr->next;
    }

    *out_size = count;
    return result;
}

int main() {
    Node n1 = {1, NULL};
    Node n2 = {4, NULL};
    Node n3 = {7, NULL};

    Sublist s1 = {&n1, NULL};
    Sublist s2 = {&n2, NULL};
    Sublist s3 = {&n3, NULL};

    s1.next = &s2;
    s2.next = &s3;

    int size = 0;
    int *first_elements = get_first_elements(&s1, &size);

    if (first_elements != NULL) {
        for (int i = 0; i < size; i++) {
            printf("%d ", first_elements[i]);
        }
        printf("\n");
        free(first_elements);
    }

    return 0;
}