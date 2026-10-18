#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
    double value;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    Node *tail;
} List;

List *list_create(void) {
    List *list = malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void list_destroy(List *list) {
    if (list == NULL) {
        return;
    }
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int list_push_back(List *list, double value) {
    if (list == NULL) {
        return -1;
    }
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        return -1;
    }
    node->value = value;
    node->next = NULL;
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    return 0;
}

List *list_map(List *list, double (*func)(double, int)) {
    if (list == NULL || func == NULL) {
        return NULL;
    }
    List *new_list = list_create();
    if (new_list == NULL) {
        return NULL;
    }
    Node *current = list->head;
    int index = 0;
    while (current != NULL) {
        double mapped_value = func(current->value, index);
        if (list_push_back(new_list, mapped_value) != 0) {
            list_destroy(new_list);
            return NULL;
        }
        current = current->next;
        index++;
    }
    return new_list;
}

double power_map(double base, int index) {
    return pow(base, index);
}

List *create_power_list(List *bases) {
    return list_map(bases, power_map);
}

int main(void) {
    List *bases = list_create();
    if (bases == NULL) {
        return EXIT_FAILURE;
    }
    
    /* Possible weaknesses found:
     *  Variable 'values' can be declared as const array [constVariable]
     */
    double values[] = {2.0, 3.0, 5.0};
    for (int i = 0; i < 3; i++) {
        if (list_push_back(bases, values[i]) != 0) {
            list_destroy(bases);
            return EXIT_FAILURE;
        }
    }
    
    List *powers = create_power_list(bases);
    if (powers == NULL) {
        list_destroy(bases);
        return EXIT_FAILURE;
    }
    
    Node *current = powers->head;
    while (current != NULL) {
        printf("%f\n", current->value);
        current = current->next;
    }
    
    list_destroy(powers);
    list_destroy(bases);
    
    return EXIT_SUCCESS;
}