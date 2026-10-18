#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode *create_node(int val) {
    struct ListNode *node = (struct ListNode *)malloc(sizeof(struct ListNode));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->val = val;
    node->next = NULL;
    return node;
}

void free_list(struct ListNode *head) {
    struct ListNode *current = head;
    struct ListNode *next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}

struct ListNode *add_two_lists(struct ListNode *l1, struct ListNode *l2) {
    struct ListNode dummy;
    dummy.val = 0;
    dummy.next = NULL;
    struct ListNode *tail = &dummy;
    int carry = 0;

    while (l1 != NULL || l2 != NULL || carry != 0) {
        int sum = carry;
        if (l1 != NULL) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2 != NULL) {
            sum += l2->val;
            l2 = l2->next;
        }
        carry = sum / 10;
        struct ListNode *new_node = create_node(sum % 10);
        tail->next = new_node;
        tail = tail->next;
    }

    return dummy.next;
}

int main(void) {
    struct ListNode *l1 = create_node(2);
    l1->next = create_node(4);
    l1->next->next = create_node(3);

    struct ListNode *l2 = create_node(5);
    l2->next = create_node(6);
    l2->next->next = create_node(4);

    struct ListNode *result = add_two_lists(l1, l2);

    struct ListNode *current = result;
    while (current != NULL) {
        printf("%d ", current->val);
        current = current->next;
    }
    printf("\n");

    free_list(l1);
    free_list(l2);
    free_list(result);

    return 0;
}