#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

/* Possible weaknesses found:
 *  Parameter 'arr' can be declared as pointer to const [constParameterPointer]
 */
Node* create_list(int *arr, int len) {
    Node *head = NULL;
    Node *tail = NULL;
    for (int i = 0; i < len; i++) {
        Node *new_node = (Node*)malloc(sizeof(Node));
        if (new_node == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        new_node->data = arr[i];
        new_node->next = NULL;
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    return head;
}

int list_length(Node *head) {
    int count = 0;
    Node *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

Node* find_longest_list(Node **lists, int num_lists) {
    if (lists == NULL || num_lists <= 0) {
        return NULL;
    }
    
    Node *longest = lists[0];
    int max_length = list_length(lists[0]);
    
    for (int i = 1; i < num_lists; i++) {
        int current_length = list_length(lists[i]);
        if (current_length > max_length) {
            max_length = current_length;
            longest = lists[i];
        }
    }
    
    return longest;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main() {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {10, 20, 30};
    int arr3[] = {100, 200, 300, 400, 500, 600, 700};
    int arr4[] = {42};
    
    Node *list1 = create_list(arr1, 5);
    Node *list2 = create_list(arr2, 3);
    Node *list3 = create_list(arr3, 7);
    Node *list4 = create_list(arr4, 1);
    
    Node *lists[] = {list1, list2, list3, list4};
    int num_lists = 4;
    
    Node *longest = find_longest_list(lists, num_lists);
    
    if (longest != NULL) {
        printf("Longest list: ");
        print_list(longest);
        printf("Length: %d\n", list_length(longest));
    } else {
        printf("No lists provided\n");
    }
    
    free_list(list1);
    free_list(list2);
    free_list(list3);
    free_list(list4);
    
    return 0;
}