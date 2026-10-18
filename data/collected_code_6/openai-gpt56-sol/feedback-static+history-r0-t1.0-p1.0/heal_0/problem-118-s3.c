#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct StringList {
    char *value;
    struct StringList *next;
} StringList;

static void free_string_list(StringList *list)
{
    while (list != NULL) {
        StringList *next = list->next;
        free(list->value);
        free(list);
        list = next;
    }
}

static char *duplicate_range(const char *start, size_t length)
{
    if (length == SIZE_MAX) {
        return NULL;
    }

    char *copy = malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, start, length);
    copy[length] = '\0';
    return copy;
}

static int string_to_list(const char *input, char delimiter, StringList **result)
{
    StringList *head = NULL;
    StringList **tail = &head;
    const char *start;

    if (input == NULL || result == NULL || delimiter == '\0') {
        errno = EINVAL;
        return -1;
    }

    *result = NULL;
    start = input;

    for (;;) {
        const char *end = strchr(start, delimiter);
        size_t length = end != NULL ? (size_t)(end - start) : strlen(start);
        StringList *node = malloc(sizeof(*node));

        if (node == NULL) {
            free_string_list(head);
            return -1;
        }

        node->value = duplicate_range(start, length);
        node->next = NULL;

        if (node->value == NULL) {
            free(node);
            free_string_list(head);
            return -1;
        }

        *tail = node;
        tail = &node->next;

        if (end == NULL) {
            break;
        }

        start = end + 1;
    }

    *result = head;
    return 0;
}

int main(int argc, char *argv[])
{
    StringList *list;
    StringList *current;
    char delimiter = ',';

    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Usage: %s <string> [delimiter]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 3) {
        if (argv[2][0] == '\0' || argv[2][1] != '\0') {
            fputs("Delimiter must be exactly one character.\n", stderr);
            return EXIT_FAILURE;
        }
        delimiter = argv[2][0];
    }

    if (string_to_list(argv[1], delimiter, &list) != 0) {
        perror("string_to_list");
        return EXIT_FAILURE;
    }

    for (current = list; current != NULL; current = current->next) {
        if (puts(current->value) == EOF) {
            perror("puts");
            free_string_list(list);
            return EXIT_FAILURE;
        }
    }

    free_string_list(list);

    if (fflush(stdout) == EOF) {
        perror("fflush");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}