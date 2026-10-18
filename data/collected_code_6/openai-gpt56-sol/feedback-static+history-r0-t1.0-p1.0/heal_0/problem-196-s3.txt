#include <stdio.h>
#include <stdlib.h>

typedef struct Tuple {
    size_t length;
    int *values;
    struct Tuple *next;
} Tuple;

static void free_tuple(Tuple *tuple)
{
    if (tuple != NULL) {
        free(tuple->values);
        free(tuple);
    }
}

static void remove_tuples_with_length(Tuple **head, size_t k)
{
    Tuple **current;

    if (head == NULL) {
        return;
    }

    current = head;

    while (*current != NULL) {
        Tuple *tuple = *current;

        if (tuple->length == k) {
            *current = tuple->next;
            free_tuple(tuple);
        } else {
            current = &tuple->next;
        }
    }
}

static void free_all_tuples(Tuple **head)
{
    Tuple *current;

    if (head == NULL) {
        return;
    }

    current = *head;
    *head = NULL;

    while (current != NULL) {
        Tuple *next = current->next;
        free_tuple(current);
        current = next;
    }
}

static int append_tuple(Tuple **head, const int *values, size_t length)
{
    Tuple *tuple;
    Tuple **tail;

    if (head == NULL || (length > 0U && values == NULL)) {
        return 0;
    }

    tuple = calloc(1U, sizeof(*tuple));
    if (tuple == NULL) {
        return 0;
    }

    if (length > 0U) {
        if (length > SIZE_MAX / sizeof(*tuple->values)) {
            free(tuple);
            return 0;
        }

        tuple->values = malloc(length * sizeof(*tuple->values));
        if (tuple->values == NULL) {
            free(tuple);
            return 0;
        }

        for (size_t i = 0U; i < length; ++i) {
            tuple->values[i] = values[i];
        }
    }

    tuple->length = length;
    tail = head;

    while (*tail != NULL) {
        tail = &(*tail)->next;
    }

    *tail = tuple;
    return 1;
}

static void print_tuples(const Tuple *head)
{
    for (const Tuple *tuple = head; tuple != NULL; tuple = tuple->next) {
        putchar('(');

        for (size_t i = 0U; i < tuple->length; ++i) {
            if (i > 0U) {
                printf(", ");
            }
            printf("%d", tuple->values[i]);
        }

        puts(")");
    }
}

int main(void)
{
    Tuple *head = NULL;
    const int first[] = {1, 2};
    const int second[] = {3, 4, 5};
    const int third[] = {6};
    const int fourth[] = {7, 8};
    const size_t k = 2U;

    if (!append_tuple(&head, first, 2U) ||
        !append_tuple(&head, second, 3U) ||
        !append_tuple(&head, third, 1U) ||
        !append_tuple(&head, fourth, 2U)) {
        fputs("Failed to allocate tuple list.\n", stderr);
        free_all_tuples(&head);
        return EXIT_FAILURE;
    }

    remove_tuples_with_length(&head, k);
    print_tuples(head);
    free_all_tuples(&head);

    return EXIT_SUCCESS;
}