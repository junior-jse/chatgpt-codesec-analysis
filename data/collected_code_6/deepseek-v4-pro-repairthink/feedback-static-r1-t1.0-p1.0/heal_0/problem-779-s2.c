#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    void *data;
    size_t data_size;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    size_t length;
    int (*compare)(const void *, const void *);
} List;

void list_init(List *list, int (*compare)(const void *, const void *)) {
    if (list == NULL || compare == NULL) {
        return;
    }
    list->head = NULL;
    list->length = 0;
    list->compare = compare;
}

int list_push_back(List *list, const void *data, size_t data_size) {
    if (list == NULL || data == NULL || data_size == 0) {
        return -1;
    }
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return -1;
    }
    new_node->data = malloc(data_size);
    if (new_node->data == NULL) {
        free(new_node);
        return -1;
    }
    memcpy(new_node->data, data, data_size);
    new_node->data_size = data_size;
    new_node->next = NULL;
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->length++;
    return 0;
}

void list_destroy(List *list) {
    if (list == NULL) {
        return;
    }
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    list->head = NULL;
    list->length = 0;
    list->compare = NULL;
}

int compare_lists(const void *a, const void *b) {
    const List *list_a = (const List *)a;
    const List *list_b = (const List *)b;
    if (list_a == NULL || list_b == NULL) {
        return 0;
    }
    if (list_a->length != list_b->length) {
        return list_a->length - list_b->length;
    }
    Node *node_a = list_a->head;
    Node *node_b = list_b->head;
    while (node_a != NULL && node_b != NULL) {
        int cmp = list_a->compare(node_a->data, node_b->data);
        if (cmp != 0) {
            return cmp;
        }
        node_a = node_a->next;
        node_b = node_b->next;
    }
    return 0;
}

int compare_ints(const void *a, const void *b) {
    const int *int_a = (const int *)a;
    const int *int_b = (const int *)b;
    return (*int_a) - (*int_b);
}

int count_unique_lists(List *list_of_lists) {
    if (list_of_lists == NULL) {
        return 0;
    }
    int count = 0;
    Node *current = list_of_lists->head;
    while (current != NULL) {
        List *current_list = (List *)current->data;
        Node *inner = list_of_lists->head;
        int is_unique = 1;
        while (inner != current) {
            List *inner_list = (List *)inner->data;
            if (compare_lists(current_list, inner_list) == 0) {
                is_unique = 0;
                break;
            }
            inner = inner->next;
        }
        if (is_unique) {
            count++;
        }
        current = current->next;
    }
    return count;
}

int main(void) {
    List list_of_lists;
    list_init(&list_of_lists, compare_lists);

    List list1, list2, list3, list4;
    list_init(&list1, compare_ints);
    list_init(&list2, compare_ints);
    list_init(&list3, compare_ints);
    list_init(&list4, compare_ints);

    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5, val6 = 6;

    list_push_back(&list1, &val1, sizeof(int));
    list_push_back(&list1, &val2, sizeof(int));

    list_push_back(&list2, &val3, sizeof(int));
    list_push_back(&list2, &val4, sizeof(int));

    list_push_back(&list3, &val5, sizeof(int));
    list_push_back(&list3, &val6, sizeof(int));

    list_push_back(&list4, &val1, sizeof(int));
    list_push_back(&list4, &val2, sizeof(int));

    list_push_back(&list_of_lists, &list1, sizeof(List));
    list_push_back(&list_of_lists, &list2, sizeof(List));
    list_push_back(&list_of_lists, &list3, sizeof(List));
    list_push_back(&list_of_lists, &list4, sizeof(List));

    int unique_count = count_unique_lists(&list_of_lists);
    printf("%d\n", unique_count);

    list_destroy(&list1);
    list_destroy(&list2);
    list_destroy(&list3);
    list_destroy(&list4);
    list_destroy(&list_of_lists);

    return 0;
}