#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
} List;

int list_sum(List *list) {
    int sum = 0;
    Node *current = list->head;
    while (current != NULL) {
        sum += current->data;
        current = current->next;
    }
    return sum;
}

List *find_max_sum_list(List **lists, int count) {
    if (lists == NULL || count <= 0) {
        return NULL;
    }
    
    List *max_list = lists[0];
    int max_sum = list_sum(max_list);
    
    for (int i = 1; i < count; i++) {
        if (lists[i] == NULL) {
            continue;
        }
        int current_sum = list_sum(lists[i]);
        if (current_sum > max_sum) {
            max_sum = current_sum;
            max_list = lists[i];
        }
    }
    
    return max_list;
}

Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

/* Possible weaknesses found:
 *  Parameter 'arr' can be declared as pointer to const [constParameterPointer]
 */
List *create_list(int *arr, int size) {
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    
    if (size == 0 || arr == NULL) {
        return list;
    }
    
    Node *current = NULL;
    for (int i = 0; i < size; i++) {
        Node *node = create_node(arr[i]);
        if (node == NULL) {
            Node *temp = list->head;
            while (temp != NULL) {
                Node *next = temp->next;
                free(temp);
                temp = next;
            }
            free(list);
            return NULL;
        }
        if (list->head == NULL) {
            list->head = node;
            current = node;
        } else {
            current->next = node;
            current = node;
        }
    }
    
    return list;
}

void free_list(List *list) {
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

void print_list(List *list) {
    if (list == NULL || list->head == NULL) {
        printf("[]\n");
        return;
    }
    
    Node *current = list->head;
    printf("[");
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("]\n");
}

int main(void) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {10, 20};
    int arr3[] = {5, 5, 5, 5};
    int arr4[] = {-1, -2, -3};
    int arr5[] = {7};
    
    List *list1 = create_list(arr1, 3);
    List *list2 = create_list(arr2, 2);
    List *list3 = create_list(arr3, 4);
    List *list4 = create_list(arr4, 3);
    List *list5 = create_list(arr5, 1);
    
    if (list1 == NULL || list2 == NULL || list3 == NULL || list4 == NULL || list5 == NULL) {
        free_list(list1);
        free_list(list2);
        free_list(list3);
        free_list(list4);
        free_list(list5);
        return 1;
    }
    
    List *lists[] = {list1, list2, list3, list4, list5};
    int count = sizeof(lists) / sizeof(lists[0]);
    
    printf("All lists:\n");
    for (int i = 0; i < count; i++) {
        printf("List %d: ", i + 1);
        print_list(lists[i]);
        printf("Sum: %d\n", list_sum(lists[i]));
    }
    
    List *max_list = find_max_sum_list(lists, count);
    printf("\nList with highest sum: ");
    print_list(max_list);
    printf("Highest sum: %d\n", list_sum(max_list));
    
    free_list(list1);
    free_list(list2);
    free_list(list3);
    free_list(list4);
    free_list(list5);
    
    return 0;
}