#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    /* Possible weaknesses found:
     *  struct member 'Node::data' is never used. [unusedStructMember]
     */
    int data;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
} List;

/* Possible weaknesses found:
 *  Parameter 'lists' can be declared as pointer to const [constParameterPointer]
 */
int count_lists(List* lists, int num_lists) {
    if (lists == NULL || num_lists <= 0) {
        return 0;
    }
    return num_lists;
}

List* create_lists(int num_lists) {
    if (num_lists <= 0) {
        return NULL;
    }
    
    List* lists = malloc((size_t)num_lists * sizeof(List));
    if (lists == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < num_lists; i++) {
        lists[i].head = NULL;
    }
    
    return lists;
}

void destroy_lists(List* lists, int num_lists) {
    if (lists == NULL) {
        return;
    }
    
    for (int i = 0; i < num_lists; i++) {
        Node* current = lists[i].head;
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    
    free(lists);
}

int main(void) {
    int num_lists = 5;
    List* lists = create_lists(num_lists);
    
    if (lists == NULL) {
        fprintf(stderr, "Failed to create lists\n");
        return EXIT_FAILURE;
    }
    
    int count = count_lists(lists, num_lists);
    printf("Number of lists: %d\n", count);
    
    destroy_lists(lists, num_lists);
    
    return EXIT_SUCCESS;
}