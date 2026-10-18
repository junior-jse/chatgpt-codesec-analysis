#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

int recursive_list_sum(struct Node *head) {
    if (head == NULL) {
        return 0;
    }
    return head->data + recursive_list_sum(head->next);
}

void free_list(struct Node *head) {
    struct Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    struct Node *head = (struct Node *)malloc(sizeof(struct Node));
    if (head == NULL) {
        return 1;
    }
    head->data = 10;

    head->next = (struct Node *)malloc(sizeof(struct Node));
    if (head->next == NULL) {
        free(head);
        return 1;
    }
    head->next->data = 20;

    head->next->next = (struct Node *)malloc(sizeof(struct Node));
    if (head->next->next == NULL) {
        free(head->next);
        free(head);
        return 1;
    }
    head->next->next->data = 30;
    head->next->next->next = NULL;

    int sum = recursive_list_sum(head);
    printf("%d\n", sum);

    free_list(head);

    return 0;
}