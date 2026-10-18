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
    size_t size;
} List;

static int compare_lists(List *a, List *b) {
    if (a->size != b->size) {
        return 0;
    }
    Node *node_a = a->head;
    Node *node_b = b->head;
    while (node_a != NULL && node_b != NULL) {
        if (node_a->data_size != node_b->data_size) {
            return 0;
        }
        if (memcmp(node_a->data, node_b->data, node_a->data_size) != 0) {
            return 0;
        }
        node_a = node_a->next;
        node_b = node_b->next;
    }
    return node_a == NULL && node_b == NULL;
}

static List *clone_list(List *src) {
    List *dst = (List *)malloc(sizeof(List));
    if (dst == NULL) {
        return NULL;
    }
    dst->head = NULL;
    dst->size = src->size;
    Node *src_node = src->head;
    Node **dst_node = &(dst->head);
    while (src_node != NULL) {
        *dst_node = (Node *)malloc(sizeof(Node));
        if (*dst_node == NULL) {
            Node *current = dst->head;
            while (current != NULL) {
                Node *next = current->next;
                free(current->data);
                free(current);
                current = next;
            }
            free(dst);
            return NULL;
        }
        (*dst_node)->data = malloc(src_node->data_size);
        if ((*dst_node)->data == NULL) {
            free(*dst_node);
            Node *current = dst->head;
            while (current != NULL) {
                Node *next = current->next;
                free(current->data);
                free(current);
                current = next;
            }
            free(dst);
            return NULL;
        }
        memcpy((*dst_node)->data, src_node->data, src_node->data_size);
        (*dst_node)->data_size = src_node->data_size;
        (*dst_node)->next = NULL;
        dst_node = &((*dst_node)->next);
        src_node = src_node->next;
    }
    return dst;
}

static void free_list(List *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}

int count_unique_lists(List **lists, size_t num_lists) {
    if (lists == NULL || num_lists == 0) {
        return 0;
    }
    List **unique_lists = (List **)malloc(num_lists * sizeof(List *));
    if (unique_lists == NULL) {
        return -1;
    }
    int unique_count = 0;
    for (size_t i = 0; i < num_lists; i++) {
        if (lists[i] == NULL) {
            free(unique_lists);
            return -1;
        }
        int is_unique = 1;
        for (int j = 0; j < unique_count; j++) {
            if (compare_lists(lists[i], unique_lists[j])) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            List *copy = clone_list(lists[i]);
            if (copy == NULL) {
                for (int k = 0; k < unique_count; k++) {
                    free_list(unique_lists[k]);
                }
                free(unique_lists);
                return -1;
            }
            unique_lists[unique_count++] = copy;
        }
    }
    for (int k = 0; k < unique_count; k++) {
        free_list(unique_lists[k]);
    }
    free(unique_lists);
    return unique_count;
}

int main(void) {
    int data1 = 10;
    int data2 = 20;
    int data3 = 30;

    Node *node1 = (Node *)malloc(sizeof(Node));
    Node *node2 = (Node *)malloc(sizeof(Node));
    Node *node3 = (Node *)malloc(sizeof(Node));
    Node *node4 = (Node *)malloc(sizeof(Node));

    node1->data = &data1;
    node1->data_size = sizeof(int);
    node1->next = node2;
    node2->data = &data2;
    node2->data_size = sizeof(int);
    node2->next = NULL;

    node3->data = &data1;
    node3->data_size = sizeof(int);
    node3->next = node4;
    node4->data = &data2;
    node4->data_size = sizeof(int);
    node4->next = NULL;

    List *list1 = (List *)malloc(sizeof(List));
    List *list2 = (List *)malloc(sizeof(List));
    List *list3 = (List *)malloc(sizeof(List));

    list1->head = node1;
    list1->size = 2;
    list2->head = node3;
    list2->size = 2;
    list3->head = NULL;
    list3->size = 0;

    List *lists[] = {list1, list2, list3};

    int result = count_unique_lists(lists, 3);
    printf("%d\n", result);

    free(node1);
    free(node2);
    free(node3);
    free(node4);
    free(list1);
    free(list2);
    free(list3);

    return 0;
}