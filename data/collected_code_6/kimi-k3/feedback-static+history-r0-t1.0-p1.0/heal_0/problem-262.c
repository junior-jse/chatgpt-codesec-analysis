#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    size_t size;
} List;

void list_init(List *list)
{
    if (list == NULL) {
        return;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

int list_append(List *list, int data)
{
    Node *new_node;

    if (list == NULL) {
        return -1;
    }

    new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return -1;
    }

    new_node->data = data;
    new_node->next = NULL;

    if (list->tail != NULL) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }

    list->tail = new_node;
    list->size++;
    return 0;
}

void list_clear(List *list)
{
    Node *current;
    Node *next;

    if (list == NULL) {
        return;
    }

    current = list->head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

int list_split(List *source, size_t split_pos, List *first, List *second)
{
    Node *current;
    Node *next;
    size_t count;

    if (source == NULL || first == NULL || second == NULL) {
        return -1;
    }

    if (first == source || second == source || first == second) {
        return -1;
    }

    list_init(first);
    list_init(second);

    if (split_pos > source->size) {
        return -1;
    }

    current = source->head;
    count = 0;

    while (current != NULL) {
        next = current->next;
        current->next = NULL;

        if (count < split_pos) {
            if (first->tail != NULL) {
                first->tail->next = current;
            } else {
                first->head = current;
            }
            first->tail = current;
            first->size++;
        } else {
            if (second->tail != NULL) {
                second->tail->next = current;
            } else {
                second->head = current;
            }
            second->tail = current;
            second->size++;
        }

        current = next;
        count++;
    }

    source->head = NULL;
    source->tail = NULL;
    source->size = 0;

    return 0;
}

void list_print(const List *list)
{
    Node *current;

    if (list == NULL) {
        printf("(null)\n");
        return;
    }

    current = list->head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

int main(void)
{
    List original;
    List first_part;
    List second_part;
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t num_values = sizeof(values) / sizeof(values[0]);
    size_t split_at = 4;
    size_t i;
    int result;

    list_init(&original);

    for (i = 0; i < num_values; i++) {
        if (list_append(&original, values[i]) != 0) {
            fprintf(stderr, "Error: Failed to allocate memory\n");
            list_clear(&original);
            return EXIT_FAILURE;
        }
    }

    printf("Original list: ");
    list_print(&original);

    result = list_split(&original, split_at, &first_part, &second_part);
    if (result != 0) {
        fprintf(stderr, "Error: Split failed (invalid position or arguments)\n");
        list_clear(&original);
        return EXIT_FAILURE;
    }

    printf("First part (%zu elements): ", split_at);
    list_print(&first_part);

    printf("Second part (%zu elements): ", second_part.size);
    list_print(&second_part);

    list_clear(&first_part);
    list_clear(&second_part);

    return EXIT_SUCCESS;
}