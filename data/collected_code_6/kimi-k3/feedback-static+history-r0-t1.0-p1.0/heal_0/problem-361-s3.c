#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    size_t size;
} List;

typedef struct {
    List* lists;
    size_t count;
    size_t capacity;
} ListOfLists;

List* list_create(void) {
    List* list = malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

void list_destroy(List* list) {
    if (list == NULL) {
        return;
    }
    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

int list_append(List* list, int data) {
    if (list == NULL) {
        return -1;
    }
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return -1;
    }
    new_node->data = data;
    new_node->next = NULL;
    
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->size++;
    return 0;
}

ListOfLists* lol_create(size_t initial_capacity) {
    ListOfLists* lol = malloc(sizeof(ListOfLists));
    if (lol == NULL) {
        return NULL;
    }
    lol->lists = calloc(initial_capacity, sizeof(List));
    if (lol->lists == NULL) {
        free(lol);
        return NULL;
    }
    lol->count = 0;
    lol->capacity = initial_capacity;
    return lol;
}

void lol_destroy(ListOfLists* lol) {
    if (lol == NULL) {
        return;
    }
    for (size_t i = 0; i < lol->count; i++) {
        Node* current = lol->lists[i].head;
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(lol->lists);
    free(lol);
}

int lol_add_list(ListOfLists* lol, List* list) {
    if (lol == NULL || list == NULL) {
        return -1;
    }
    if (lol->count >= lol->capacity) {
        size_t new_capacity = lol->capacity * 2;
        List* new_lists = realloc(lol->lists, new_capacity * sizeof(List));
        if (new_lists == NULL) {
            return -1;
        }
        lol->lists = new_lists;
        lol->capacity = new_capacity;
    }
    lol->lists[lol->count] = *list;
    free(list);
    lol->count++;
    return 0;
}

int remove_empty_lists(ListOfLists* lol) {
    if (lol == NULL) {
        return -1;
    }
    size_t write_idx = 0;
    for (size_t read_idx = 0; read_idx < lol->count; read_idx++) {
        if (lol->lists[read_idx].head != NULL) {
            if (write_idx != read_idx) {
                lol->lists[write_idx] = lol->lists[read_idx];
            }
            write_idx++;
        } else {
            Node* current = lol->lists[read_idx].head;
            while (current != NULL) {
                Node* temp = current;
                current = current->next;
                free(temp);
            }
        }
    }
    lol->count = write_idx;
    return 0;
}

void print_list(const List* list) {
    if (list == NULL) {
        return;
    }
    Node* current = list->head;
    printf("[");
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("]");
}

void print_lol(const ListOfLists* lol) {
    if (lol == NULL) {
        return;
    }
    printf("[");
    for (size_t i = 0; i < lol->count; i++) {
        print_list(&lol->lists[i]);
        if (i < lol->count - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(void) {
    ListOfLists* lol = lol_create(4);
    if (lol == NULL) {
        fprintf(stderr, "Failed to create list of lists\n");
        return EXIT_FAILURE;
    }
    
    List* list1 = list_create();
    if (list1 == NULL) {
        lol_destroy(lol);
        return EXIT_FAILURE;
    }
    if (list_append(list1, 1) != 0 || list_append(list1, 2) != 0) {
        list_destroy(list1);
        lol_destroy(lol);
        return EXIT_FAILURE;
    }
    
    List* list2 = list_create();
    if (list2 == NULL) {
        list_destroy(list1);
        lol_destroy(lol);
        return EXIT_FAILURE;
    }
    
    List* list3 = list_create();
    if (list3 == NULL) {
        list_destroy(list1);
        list_destroy(list2);
        lol_destroy(lol);
        return EXIT_FAILURE;
    }
    if (list_append(list3, 3) != 0) {
        list_destroy(list1);
        list_destroy(list2);
        list_destroy(list3);
        lol_destroy(lol);
        return EXIT_FAILURE;
    }
    
    List* list4 = list_create();
    if (list4 == NULL) {
        list_destroy(list1);
        list_destroy(list2);
        list_destroy(list3);
        lol_destroy(lol);
        return EXIT_FAILURE;
    }
    
    if (lol_add_list(lol, list1) != 0 ||
        lol_add_list(lol, list2) != 0 ||
        lol_add_list(lol, list3) != 0 ||
        lol_add_list(lol, list4) != 0) {
        lol_destroy(lol);
        return EXIT_FAILURE;
    }
    
    printf("Original: ");
    print_lol(lol);
    
    if (remove_empty_lists(lol) != 0) {
        lol_destroy(lol);
        return EXIT_FAILURE;
    }
    
    printf("After removal: ");
    print_lol(lol);
    
    lol_destroy(lol);
    return EXIT_SUCCESS;
}