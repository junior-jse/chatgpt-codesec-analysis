#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    struct List* next;
} List;

int list_equals(Node* a, Node* b) {
    while (a != NULL && b != NULL) {
        if (a->data != b->data) {
            return 0;
        }
        a = a->next;
        b = b->next;
    }
    return a == NULL && b == NULL;
}

void remove_duplicates(List* head) {
    if (head == NULL) {
        return;
    }

    List* current = head;
    while (current != NULL) {
        List* runner = current;
        while (runner->next != NULL) {
            if (list_equals(current->head, runner->next->head)) {
                List* duplicate = runner->next;
                runner->next = duplicate->next;
                Node* node = duplicate->head;
                while (node != NULL) {
                    Node* temp = node;
                    node = node->next;
                    free(temp);
                }
                free(duplicate);
            } else {
                runner = runner->next;
            }
        }
        current = current->next;
    }
}

void free_list_of_lists(List* head) {
    while (head != NULL) {
        List* next_list = head->next;
        Node* node = head->head;
        while (node != NULL) {
            Node* temp = node;
            node = node->next;
            free(temp);
        }
        free(head);
        head = next_list;
    }
}

Node* create_node(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

List* create_list_node(Node* head) {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = head;
    list->next = NULL;
    return list;
}

int main(void) {
    Node* list1_node1 = create_node(1);
    Node* list1_node2 = create_node(2);
    Node* list1_node3 = create_node(3);
    if (list1_node1 == NULL || list1_node2 == NULL || list1_node3 == NULL) {
        free_list_of_lists(NULL);
        return EXIT_FAILURE;
    }
    list1_node1->next = list1_node2;
    list1_node2->next = list1_node3;

    Node* list2_node1 = create_node(1);
    Node* list2_node2 = create_node(2);
    Node* list2_node3 = create_node(3);
    if (list2_node1 == NULL || list2_node2 == NULL || list2_node3 == NULL) {
        free_list_of_lists(NULL);
        return EXIT_FAILURE;
    }
    list2_node1->next = list2_node2;
    list2_node2->next = list2_node3;

    Node* list3_node1 = create_node(4);
    Node* list3_node2 = create_node(5);
    if (list3_node1 == NULL || list3_node2 == NULL) {
        free_list_of_lists(NULL);
        return EXIT_FAILURE;
    }
    list3_node1->next = list3_node2;

    List* list1 = create_list_node(list1_node1);
    List* list2 = create_list_node(list2_node1);
    List* list3 = create_list_node(list3_node1);
    if (list1 == NULL || list2 == NULL || list3 == NULL) {
        free_list_of_lists(NULL);
        return EXIT_FAILURE;
    }
    list1->next = list2;
    list2->next = list3;

    remove_duplicates(list1);

    List* temp = list1;
    while (temp != NULL) {
        Node* node = temp->head;
        while (node != NULL) {
            printf("%d ", node->data);
            node = node->next;
        }
        printf("\n");
        temp = temp->next;
    }

    free_list_of_lists(list1);
    return EXIT_SUCCESS;
}