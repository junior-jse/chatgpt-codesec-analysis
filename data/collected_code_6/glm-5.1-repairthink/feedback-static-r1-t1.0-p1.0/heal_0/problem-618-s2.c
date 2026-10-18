#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

Node* divide_lists(Node* head1, Node* head2) {
    Node* result_head = NULL;
    Node* result_tail = NULL;

    while (head1 != NULL && head2 != NULL) {
        if (head2->data == 0) {
            Node* temp = result_head;
            while (temp != NULL) {
                Node* next = temp->next;
                free(temp);
                temp = next;
            }
            return NULL;
        }

        Node* new_node = create_node(head1->data / head2->data);

        if (result_head == NULL) {
            result_head = new_node;
            result_tail = new_node;
        } else {
            result_tail->next = new_node;
            result_tail = new_node;
        }

        head1 = head1->next;
        head2 = head2->next;
    }

    return result_head;
}

void free_list(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void print_list(Node* head) {
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

int main(void) {
    Node* list1 = NULL;
    Node* list1_tail = NULL;
    Node* list2 = NULL;
    Node* list2_tail = NULL;

    int data1[] = {10, 20, 30, 40, 50};
    int data2[] = {2, 4, 5, 10, 5};
    size_t len = sizeof(data1) / sizeof(data1[0]);

    for (size_t i = 0; i < len; ++i) {
        Node* n1 = create_node(data1[i]);
        if (list1 == NULL) {
            list1 = n1;
            list1_tail = n1;
        } else {
            list1_tail->next = n1;
            list1_tail = n1;
        }

        Node* n2 = create_node(data2[i]);
        if (list2 == NULL) {
            list2 = n2;
            list2_tail = n2;
        } else {
            list2_tail->next = n2;
            list2_tail = n2;
        }
    }

    Node* result = divide_lists(list1, list2);

    if (result != NULL) {
        print_list(result);
        free_list(result);
    }

    free_list(list1);
    free_list(list2);

    return 0;
}