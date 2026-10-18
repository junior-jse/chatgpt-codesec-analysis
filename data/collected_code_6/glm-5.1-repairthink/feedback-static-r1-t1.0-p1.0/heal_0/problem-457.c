#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node** sublists;
    int* lengths;
    int count;
} SublistResult;

SublistResult find_min_length_sublist(Node* head) {
    SublistResult result = {NULL, NULL, 0};
    if (head == NULL) {
        return result;
    }

    int total_sublists = 0;
    Node* curr = head;
    while (curr != NULL) {
        if (curr->data < 0) {
            total_sublists++;
        }
        curr = curr->next;
    }

    if (total_sublists == 0) {
        return result;
    }

    result.sublists = (Node**)malloc(total_sublists * sizeof(Node*));
    result.lengths = (int*)malloc(total_sublists * sizeof(int));
    if (result.sublists == NULL || result.lengths == NULL) {
        free(result.sublists);
        free(result.lengths);
        result.sublists = NULL;
        result.lengths = NULL;
        return result;
    }

    result.count = total_sublists;
    curr = head;
    int idx = 0;
    while (curr != NULL) {
        if (curr->data < 0) {
            result.sublists[idx] = curr;
            int len = 1;
            Node* temp = curr->next;
            while (temp != NULL && temp->data >= 0) {
                len++;
                temp = temp->next;
            }
            result.lengths[idx] = len;
            idx++;
            curr = temp;
        } else {
            curr = curr->next;
        }
    }

    int min_len = result.lengths[0];
    for (int i = 1; i < result.count; i++) {
        if (result.lengths[i] < min_len) {
            min_len = result.lengths[i];
        }
    }

    return result;
}

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_list(Node* head) {
    Node* curr = head;
    while (curr != NULL) {
        Node* next = curr->next;
        free(curr);
        curr = next;
    }
}

void free_sublist_result(SublistResult result) {
    free(result.sublists);
    free(result.lengths);
}

int main() {
    Node* head = create_node(-1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(-4);
    head->next->next->next->next = create_node(5);
    head->next->next->next->next->next = create_node(-6);

    SublistResult result = find_min_length_sublist(head);

    if (result.count == 0) {
        printf("No sublists found\n");
    } else {
        for (int i = 0; i < result.count; i++) {
            if (result.lengths[i] == result.lengths[0]) {
                printf("Minimum length sublist %d: ", i + 1);
                Node* curr = result.sublists[i];
                for (int j = 0; j < result.lengths[i]; j++) {
                    printf("%d ", curr->data);
                    curr = curr->next;
                }
                printf("\n");
            }
        }
    }

    free_sublist_result(result);
    free_list(head);

    return 0;
}