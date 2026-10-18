#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int key;
    int value;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} Dictionary;

bool all_values_same(Dictionary* dict) {
    if (dict == NULL) {
        return true;
    }

    if (dict->head == NULL) {
        return true;
    }

    int first_value = dict->head->value;
    Node* current = dict->head->next;

    while (current != NULL) {
        if (current->value != first_value) {
            return false;
        }
        current = current->next;
    }

    return true;
}

void free_dictionary(Dictionary* dict) {
    if (dict == NULL) {
        return;
    }

    Node* current = dict->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    dict->head = NULL;
}

int main() {
    Dictionary dict1 = {NULL};
    bool result1 = all_values_same(&dict1);
    printf("%d\n", result1);

    Node* n1 = malloc(sizeof(Node));
    Node* n2 = malloc(sizeof(Node));
    Node* n3 = malloc(sizeof(Node));

    if (n1 == NULL || n2 == NULL || n3 == NULL) {
        free(n1);
        free(n2);
        free(n3);
        return EXIT_FAILURE;
    }

    n1->key = 1; n1->value = 10; n1->next = n2;
    n2->key = 2; n2->value = 10; n2->next = n3;
    n3->key = 3; n3->value = 10; n3->next = NULL;
    dict1.head = n1;

    bool result2 = all_values_same(&dict1);
    printf("%d\n", result2);

    dict1.head->next->value = 20;

    bool result3 = all_values_same(&dict1);
    printf("%d\n", result3);

    free_dictionary(&dict1);

    return EXIT_SUCCESS;
}