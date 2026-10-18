#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

int list_sum(struct Node *head) {
    if (head == NULL) {
        return 0;
    }
    return head->data + list_sum(head->next);
}

void free_list(struct Node *head) {
    while (head != NULL) {
        struct Node *temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    struct Node *head = malloc(sizeof(struct Node));
    if (head == NULL) {
        return 1;
    }
    head->data = 1;
    
    head->next = malloc(sizeof(struct Node));
    if (head->next == NULL) {
        free(head);
        return 1;
    }
    head->next->data = 2;
    
    head->next->next = malloc(sizeof(struct Node));
    if (head->next->next == NULL) {
        free(head->next);
        free(head);
        return 1;
    }
    head->next->next->data = 3;
    head->next->next->next = NULL;
    
    int sum = list_sum(head);
    printf("%d\n", sum);
    
    free_list(head);
    
    return 0;
}