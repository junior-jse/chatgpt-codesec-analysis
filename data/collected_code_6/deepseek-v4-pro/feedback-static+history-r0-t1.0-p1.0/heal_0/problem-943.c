#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) {
    if (list1 == NULL) return list2;
    if (list2 == NULL) return list1;

    struct ListNode *head = NULL;
    struct ListNode *tail = NULL;

    if (list1->val <= list2->val) {
        head = list1;
        list1 = list1->next;
    } else {
        head = list2;
        list2 = list2->next;
    }
    tail = head;

    while (list1 != NULL && list2 != NULL) {
        if (list1->val <= list2->val) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }

    if (list1 != NULL) {
        tail->next = list1;
    } else {
        tail->next = list2;
    }

    return head;
}

struct ListNode* createNode(int val) {
    struct ListNode *node = (struct ListNode*)malloc(sizeof(struct ListNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->val = val;
    node->next = NULL;
    return node;
}

void printList(struct ListNode *head) {
    struct ListNode *current = head;
    while (current != NULL) {
        printf("%d ", current->val);
        current = current->next;
    }
    printf("\n");
}

void freeList(struct ListNode *head) {
    struct ListNode *current = head;
    while (current != NULL) {
        struct ListNode *temp = current;
        current = current->next;
        free(temp);
    }
}

int main(void) {
    struct ListNode *list1 = createNode(1);
    list1->next = createNode(3);
    list1->next->next = createNode(5);

    struct ListNode *list2 = createNode(2);
    list2->next = createNode(4);
    list2->next->next = createNode(6);

    struct ListNode *merged = mergeTwoLists(list1, list2);
    printList(merged);
    freeList(merged);

    struct ListNode *empty1 = NULL;
    struct ListNode *empty2 = NULL;
    struct ListNode *mergedEmpty = mergeTwoLists(empty1, empty2);
    printList(mergedEmpty);
    freeList(mergedEmpty);

    return 0;
}