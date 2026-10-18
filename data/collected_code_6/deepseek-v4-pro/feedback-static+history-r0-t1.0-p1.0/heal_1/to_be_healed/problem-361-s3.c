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

/* Possible weaknesses found:
 *  Parameter 'list' can be declared as pointer to const [constParameterPointer]
 */
int is_empty(List* list) {
    if (list == NULL) {
        return 1;
    }
    return list->head == NULL;
}

List* remove_empty_lists(List* list_of_lists) {
    if (list_of_lists == NULL) {
        return NULL;
    }
    
    List* dummy = (List*)malloc(sizeof(List));
    if (dummy == NULL) {
        return list_of_lists;
    }
    
    dummy->next = list_of_lists;
    List* prev = dummy;
    List* current = list_of_lists;
    
    while (current != NULL) {
        if (is_empty(current)) {
            prev->next = current->next;
            List* to_free = current;
            current = current->next;
            if (to_free->head != NULL) {
                Node* node = to_free->head;
                while (node != NULL) {
                    Node* temp = node;
                    node = node->next;
                    free(temp);
                }
            }
            free(to_free);
        } else {
            prev = current;
            current = current->next;
        }
    }
    
    List* result = dummy->next;
    free(dummy);
    return result;
}

/* Possible weaknesses found:
 *  Parameter 'arr' can be declared as pointer to const [constParameterPointer]
 */
List* create_list(int* arr, int size) {
    if (size <= 0) {
        List* list = (List*)malloc(sizeof(List));
        if (list == NULL) {
            return NULL;
        }
        list->head = NULL;
        list->next = NULL;
        return list;
    }
    
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->next = NULL;
    
    Node* tail = NULL;
    for (int i = 0; i < size; i++) {
        Node* node = (Node*)malloc(sizeof(Node));
        if (node == NULL) {
            Node* current = list->head;
            while (current != NULL) {
                Node* temp = current;
                current = current->next;
                free(temp);
            }
            free(list);
            return NULL;
        }
        node->data = arr[i];
        node->next = NULL;
        if (list->head == NULL) {
            list->head = node;
        } else {
            tail->next = node;
        }
        tail = node;
    }
    
    return list;
}

void add_list_to_list(List** head, List* new_list) {
    if (new_list == NULL) {
        return;
    }
    new_list->next = NULL;
    if (*head == NULL) {
        *head = new_list;
    } else {
        List* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_list;
    }
}

void print_list(List* list) {
    if (list == NULL) {
        printf("(null) ");
        return;
    }
    
    if (list->head == NULL) {
        printf("[] ");
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
    printf("] ");
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

int main(void) {
    List* list_of_lists = NULL;
    
    int arr1[] = {1, 2, 3};
    List* list1 = create_list(arr1, 3);
    
    int arr2[] = {};
    List* list2 = create_list(arr2, 0);
    
    int arr3[] = {4, 5};
    List* list3 = create_list(arr3, 2);
    
    List* list4 = create_list(NULL, 0);
    
    int arr5[] = {6};
    List* list5 = create_list(arr5, 1);
    
    add_list_to_list(&list_of_lists, list1);
    add_list_to_list(&list_of_lists, list2);
    add_list_to_list(&list_of_lists, list3);
    add_list_to_list(&list_of_lists, list4);
    add_list_to_list(&list_of_lists, list5);
    
    printf("Original list: ");
    List* temp = list_of_lists;
    while (temp != NULL) {
        print_list(temp);
        temp = temp->next;
    }
    printf("\n");
    
    list_of_lists = remove_empty_lists(list_of_lists);
    
    printf("After removing empty lists: ");
    temp = list_of_lists;
    while (temp != NULL) {
        print_list(temp);
        temp = temp->next;
    }
    printf("\n");
    
    free_list_of_lists(list_of_lists);
    
    return 0;
}