#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node **sublists;
    size_t count;
    size_t capacity;
} SplitResult;

static Node *create_node(int data) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

static void free_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

static void free_split_result(SplitResult *result) {
    if (result == NULL) {
        return;
    }
    if (result->sublists != NULL) {
        for (size_t i = 0; i < result->count; i++) {
            free_list(result->sublists[i]);
        }
        free(result->sublists);
    }
    result->sublists = NULL;
    result->count = 0;
    result->capacity = 0;
}

static int append_to_result(SplitResult *result, Node *sublist) {
    if (result->count == result->capacity) {
        size_t new_capacity = result->capacity == 0 ? 4 : result->capacity * 2;
        Node **new_sublists = realloc(result->sublists, new_capacity * sizeof(Node *));
        if (new_sublists == NULL) {
            return -1;
        }
        result->sublists = new_sublists;
        result->capacity = new_capacity;
    }
    result->sublists[result->count] = sublist;
    result->count++;
    return 0;
}

static int split_list_every_nth(Node *head, size_t n, SplitResult *result) {
    if (result == NULL || n == 0) {
        return -1;
    }

    result->sublists = NULL;
    result->count = 0;
    result->capacity = 0;

    if (head == NULL) {
        return 0;
    }

    Node *current = head;
    Node *sublist_head = NULL;
    Node *sublist_tail = NULL;
    size_t count = 0;

    while (current != NULL) {
        Node *new_node = create_node(current->data);
        if (new_node == NULL) {
            free_list(sublist_head);
            free_split_result(result);
            return -1;
        }

        if (sublist_head == NULL) {
            sublist_head = new_node;
            sublist_tail = new_node;
        } else {
            sublist_tail->next = new_node;
            sublist_tail = new_node;
        }

        count++;
        current = current->next;

        if (count == n || current == NULL) {
            if (append_to_result(result, sublist_head) != 0) {
                free_list(sublist_head);
                free_split_result(result);
                return -1;
            }
            sublist_head = NULL;
            sublist_tail = NULL;
            count = 0;
        }
    }

    return 0;
}

static void print_list(const Node *head) {
    printf("[");
    while (head != NULL) {
        printf("%d", head->data);
        if (head->next != NULL) {
            printf(", ");
        }
        head = head->next;
    }
    printf("]");
}

static void print_split_result(const SplitResult *result) {
    printf("Split into %zu sublists:\n", result->count);
    for (size_t i = 0; i < result->count; i++) {
        printf("  Sublist %zu: ", i + 1);
        print_list(result->sublists[i]);
        printf("\n");
    }
}

static Node *create_sample_list(const int *values, size_t count) {
    Node *head = NULL;
    Node *tail = NULL;

    for (size_t i = 0; i < count; i++) {
        Node *node = create_node(values[i]);
        if (node == NULL) {
            free_list(head);
            return NULL;
        }
        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

int main(void) {
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    size_t num_values = sizeof(values) / sizeof(values[0]);
    size_t n = 3;

    Node *head = create_sample_list(values, num_values);
    if (head == NULL) {
        fprintf(stderr, "Failed to create sample list\n");
        return EXIT_FAILURE;
    }

    printf("Original list: ");
    print_list(head);
    printf("\n\n");

    SplitResult result;
    if (split_list_every_nth(head, n, &result) != 0) {
        fprintf(stderr, "Failed to split list\n");
        free_list(head);
        return EXIT_FAILURE;
    }

    print_split_result(&result);

    free_split_result(&result);
    free_list(head);

    return EXIT_SUCCESS;
}