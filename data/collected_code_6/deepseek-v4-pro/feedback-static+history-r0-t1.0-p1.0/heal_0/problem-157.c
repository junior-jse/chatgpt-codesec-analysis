#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode {
    int value;
    struct ListNode *next;
} ListNode;

typedef struct RLEPair {
    int value;
    int count;
    struct RLEPair *next;
} RLEPair;

ListNode *create_node(int value) {
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    return node;
}

RLEPair *create_rle_pair(int value, int count) {
    RLEPair *pair = (RLEPair *)malloc(sizeof(RLEPair));
    if (pair == NULL) {
        return NULL;
    }
    pair->value = value;
    pair->count = count;
    pair->next = NULL;
    return pair;
}

void free_list(ListNode *head) {
    ListNode *current = head;
    while (current != NULL) {
        ListNode *next = current->next;
        free(current);
        current = next;
    }
}

void free_rle_list(RLEPair *head) {
    RLEPair *current = head;
    while (current != NULL) {
        RLEPair *next = current->next;
        free(current);
        current = next;
    }
}

RLEPair *run_length_encode(ListNode *head) {
    if (head == NULL) {
        return NULL;
    }

    RLEPair *rle_head = NULL;
    RLEPair *rle_tail = NULL;
    ListNode *current = head;

    while (current != NULL) {
        int value = current->value;
        int count = 1;
        current = current->next;

        while (current != NULL && current->value == value) {
            count++;
            current = current->next;
        }

        RLEPair *pair = create_rle_pair(value, count);
        if (pair == NULL) {
            free_rle_list(rle_head);
            return NULL;
        }

        if (rle_head == NULL) {
            rle_head = pair;
            rle_tail = pair;
        } else {
            rle_tail->next = pair;
            rle_tail = pair;
        }
    }

    return rle_head;
}

void print_rle_list(RLEPair *head) {
    RLEPair *current = head;
    while (current != NULL) {
        printf("(%d,%d) ", current->value, current->count);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    ListNode *head = NULL;
    ListNode *tail = NULL;
    int values[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
    size_t n = sizeof(values) / sizeof(values[0]);
    size_t i;

    for (i = 0; i < n; i++) {
        ListNode *node = create_node(values[i]);
        if (node == NULL) {
            free_list(head);
            return EXIT_FAILURE;
        }
        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    RLEPair *rle = run_length_encode(head);
    if (rle == NULL) {
        free_list(head);
        return EXIT_FAILURE;
    }

    print_rle_list(rle);

    free_rle_list(rle);
    free_list(head);

    return EXIT_SUCCESS;
}