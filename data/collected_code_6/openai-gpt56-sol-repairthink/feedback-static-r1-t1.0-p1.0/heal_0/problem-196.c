#include <stdio.h>
#include <stdlib.h>

typedef struct Tuple {
    int *values;
    size_t length;
    struct Tuple *next;
} Tuple;

int append_tuple(Tuple **head, const int *values, size_t length)
{
    Tuple *node;
    Tuple **tail;

    if (head == NULL || (length > 0 && values == NULL)) {
        return 0;
    }

    node = calloc(1, sizeof(*node));
    if (node == NULL) {
        return 0;
    }

    if (length > 0) {
        if (length > SIZE_MAX / sizeof(*node->values)) {
            free(node);
            return 0;
        }

        node->values = malloc(length * sizeof(*node->values));
        if (node->values == NULL) {
            free(node);
            return 0;
        }

        for (size_t i = 0; i < length; ++i) {
            node->values[i] = values[i];
        }
    }

    node->length = length;
    tail = head;

    while (*tail != NULL) {
        tail = &(*tail)->next;
    }

    *tail = node;
    return 1;
}

void remove_tuples_with_length(Tuple **head, size_t k)
{
    Tuple **link;

    if (head == NULL) {
        return;
    }

    link = head;

    while (*link != NULL) {
        Tuple *current = *link;

        if (current->length == k) {
            *link = current->next;
            free(current->values);
            free(current);
        } else {
            link = &current->next;
        }
    }
}

void free_tuples(Tuple **head)
{
    Tuple *current;

    if (head == NULL) {
        return;
    }

    current = *head;

    while (current != NULL) {
        Tuple *next = current->next;
        free(current->values);
        free(current);
        current = next;
    }

    *head = NULL;
}

void print_tuples(const Tuple *head)
{
    for (const Tuple *tuple = head; tuple != NULL; tuple = tuple->next) {
        putchar('(');

        for (size_t i = 0; i < tuple->length; ++i) {
            if (i > 0) {
                printf(", ");
            }
            printf("%d", tuple->values[i]);
        }

        puts(")");
    }
}

int main(void)
{
    Tuple *tuples = NULL;
    int first[] = {1, 2};
    int second[] = {3, 4, 5};
    int third[] = {6, 7};
    size_t k = 2;

    if (!append_tuple(&tuples, first, sizeof(first) / sizeof(first[0])) ||
        !append_tuple(&tuples, second, sizeof(second) / sizeof(second[0])) ||
        !append_tuple(&tuples, third, sizeof(third) / sizeof(third[0]))) {
        fputs("Failed to allocate tuple storage\n", stderr);
        free_tuples(&tuples);
        return EXIT_FAILURE;
    }

    remove_tuples_with_length(&tuples, k);
    print_tuples(tuples);
    free_tuples(&tuples);

    return EXIT_SUCCESS;
}