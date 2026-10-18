#include <stdio.h>
#include <stdlib.h>

typedef struct Tuple {
    int *values;
    size_t length;
    struct Tuple *next;
} Tuple;

static void free_tuple(Tuple *tuple)
{
    if (tuple != NULL) {
        free(tuple->values);
        free(tuple);
    }
}

static void free_all_tuples(Tuple *head)
{
    while (head != NULL) {
        Tuple *next = head->next;
        free_tuple(head);
        head = next;
    }
}

static Tuple *create_tuple(const int *values, size_t length)
{
    Tuple *tuple = malloc(sizeof(*tuple));

    if (tuple == NULL) {
        return NULL;
    }

    tuple->values = NULL;
    tuple->length = length;
    tuple->next = NULL;

    if (length > 0) {
        if (values == NULL || length > SIZE_MAX / sizeof(*tuple->values)) {
            free(tuple);
            return NULL;
        }

        tuple->values = malloc(length * sizeof(*tuple->values));
        if (tuple->values == NULL) {
            free(tuple);
            return NULL;
        }

        for (size_t i = 0; i < length; ++i) {
            tuple->values[i] = values[i];
        }
    }

    return tuple;
}

static int append_tuple(Tuple **head, Tuple **tail, const int *values, size_t length)
{
    Tuple *tuple;

    if (head == NULL || tail == NULL) {
        return 0;
    }

    tuple = create_tuple(values, length);
    if (tuple == NULL) {
        return 0;
    }

    if (*tail == NULL) {
        *head = tuple;
    } else {
        (*tail)->next = tuple;
    }

    *tail = tuple;
    return 1;
}

static void remove_tuples_with_length(Tuple **head, size_t k)
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
            free_tuple(current);
        } else {
            link = &current->next;
        }
    }
}

static void print_tuples(const Tuple *head)
{
    for (const Tuple *tuple = head; tuple != NULL; tuple = tuple->next) {
        putchar('(');

        for (size_t i = 0; i < tuple->length; ++i) {
            if (i > 0) {
                fputs(", ", stdout);
            }
            printf("%d", tuple->values[i]);
        }

        puts(")");
    }
}

int main(void)
{
    Tuple *head = NULL;
    Tuple *tail = NULL;
    int first[] = {1, 2};
    int second[] = {3, 4, 5};
    int third[] = {6, 7};
    int fourth[] = {8};
    size_t k = 2;

    if (!append_tuple(&head, &tail, first, 2) ||
        !append_tuple(&head, &tail, second, 3) ||
        !append_tuple(&head, &tail, third, 2) ||
        !append_tuple(&head, &tail, fourth, 1)) {
        fputs("Failed to allocate tuple storage.\n", stderr);
        free_all_tuples(head);
        return EXIT_FAILURE;
    }

    remove_tuples_with_length(&head, k);
    print_tuples(head);
    free_all_tuples(head);

    return EXIT_SUCCESS;
}