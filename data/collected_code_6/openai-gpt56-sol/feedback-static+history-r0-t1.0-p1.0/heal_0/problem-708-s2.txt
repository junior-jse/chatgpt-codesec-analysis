#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct StringList {
    char *value;
    struct StringList *next;
} StringList;

static void free_string_list(StringList *head)
{
    while (head != NULL) {
        StringList *next = head->next;
        free(head->value);
        free(head);
        head = next;
    }
}

static char *duplicate_range(const char *start, size_t length)
{
    char *copy;

    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    if (length > 0) {
        memcpy(copy, start, length);
    }
    copy[length] = '\0';

    return copy;
}

static int string_to_list(const char *input, char delimiter, StringList **result)
{
    const char *start;
    const char *current;
    StringList *head = NULL;
    StringList *tail = NULL;

    if (input == NULL || result == NULL || delimiter == '\0') {
        return EINVAL;
    }

    *result = NULL;
    start = input;
    current = input;

    for (;;) {
        if (*current == delimiter || *current == '\0') {
            StringList *node = malloc(sizeof(*node));

            if (node == NULL) {
                free_string_list(head);
                return ENOMEM;
            }

            node->value = duplicate_range(start, (size_t)(current - start));
            node->next = NULL;

            if (node->value == NULL) {
                free(node);
                free_string_list(head);
                return ENOMEM;
            }

            if (tail == NULL) {
                head = node;
            } else {
                tail->next = node;
            }
            tail = node;

            if (*current == '\0') {
                break;
            }

            start = current + 1;
        }

        current++;
    }

    *result = head;
    return 0;
}

int main(void)
{
    const char *input = "alpha,beta,gamma,delta";
    StringList *list = NULL;
    StringList *current;
    int status = string_to_list(input, ',', &list);

    if (status != 0) {
        fprintf(stderr, "Failed to convert string to list: %s\n", strerror(status));
        return EXIT_FAILURE;
    }

    for (current = list; current != NULL; current = current->next) {
        if (printf("%s\n", current->value) < 0) {
            free_string_list(list);
            return EXIT_FAILURE;
        }
    }

    free_string_list(list);
    return EXIT_SUCCESS;
}