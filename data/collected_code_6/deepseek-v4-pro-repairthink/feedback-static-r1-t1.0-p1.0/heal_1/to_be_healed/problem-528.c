#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    int length;
    struct List* next;
} List;

/* Possible weaknesses found:
 *  Parameter 'arr' can be declared as pointer to const [constParameterPointer]
 */
List* create_list(int* arr, int size) {
    List* list = (List*)malloc(sizeof(List));
    if (!list) return NULL;
    list->head = NULL;
    list->length = size;
    list->next = NULL;
    
    Node* tail = NULL;
    for (int i = 0; i < size; i++) {
        Node* node = (Node*)malloc(sizeof(Node));
        if (!node) {
            Node* current = list->head;
            while (current) {
                Node* temp = current;
                current = current->next;
                free(temp);
            }
            free(list);
            return NULL;
        }
        node->data = arr[i];
        node->next = NULL;
        if (!list->head) {
            list->head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return list;
}

void free_list(List* list) {
    if (!list) return;
    Node* current = list->head;
    while (current) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

List* append_list(List* head, List* new_list) {
    if (!head) return new_list;
    List* current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = new_list;
    return head;
}

List* find_min_length_lists(List* lists) {
    if (!lists) return NULL;
    
    int min_length = lists->length;
    List* current = lists;
    while (current) {
        if (current->length < min_length) {
            min_length = current->length;
        }
        current = current->next;
    }
    
    List* result = NULL;
    List* result_tail = NULL;
    current = lists;
    while (current) {
        if (current->length == min_length) {
            List* copy = (List*)malloc(sizeof(List));
            if (!copy) {
                free_list(result);
                return NULL;
            }
            copy->head = NULL;
            copy->length = current->length;
            copy->next = NULL;
            
            Node* src_node = current->head;
            Node* dst_tail = NULL;
            while (src_node) {
                Node* node = (Node*)malloc(sizeof(Node));
                if (!node) {
                    free_list(copy);
                    free_list(result);
                    return NULL;
                }
                node->data = src_node->data;
                node->next = NULL;
                if (!copy->head) {
                    copy->head = node;
                    dst_tail = node;
                } else {
                    dst_tail->next = node;
                    dst_tail = node;
                }
                src_node = src_node->next;
            }
            
            if (!result) {
                result = copy;
                result_tail = copy;
            } else {
                result_tail->next = copy;
                result_tail = copy;
            }
        }
        current = current->next;
    }
    return result;
}

void print_lists(List* lists) {
    List* current_list = lists;
    int list_index = 1;
    while (current_list) {
        printf("List %d (length %d): ", list_index++, current_list->length);
        Node* current_node = current_list->head;
        while (current_node) {
            printf("%d ", current_node->data);
            current_node = current_node->next;
        }
        printf("\n");
        current_list = current_list->next;
    }
}

int main() {
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5};
    int arr3[] = {6, 7, 8, 9};
    int arr4[] = {10};
    int arr5[] = {11, 12};
    
    List* list1 = create_list(arr1, 3);
    List* list2 = create_list(arr2, 2);
    List* list3 = create_list(arr3, 4);
    List* list4 = create_list(arr4, 1);
    List* list5 = create_list(arr5, 2);
    
    if (!list1 || !list2 || !list3 || !list4 || !list5) {
        free_list(list1);
        free_list(list2);
        free_list(list3);
        free_list(list4);
        free_list(list5);
        return 1;
    }
    
    List* all_lists = NULL;
    all_lists = append_list(all_lists, list1);
    all_lists = append_list(all_lists, list2);
    all_lists = append_list(all_lists, list3);
    all_lists = append_list(all_lists, list4);
    all_lists = append_list(all_lists, list5);
    
    printf("All lists:\n");
    print_lists(all_lists);
    
    List* min_lists = find_min_length_lists(all_lists);
    printf("\nLists with minimum length:\n");
    print_lists(min_lists);
    
    free_list(all_lists);
    free_list(min_lists);
    
    return 0;
}