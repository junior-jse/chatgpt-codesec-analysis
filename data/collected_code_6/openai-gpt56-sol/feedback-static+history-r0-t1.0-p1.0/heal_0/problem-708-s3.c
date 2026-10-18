#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct StringNode {
    char *value;
    struct StringNode *next;
} StringNode;

static void free_list(StringNode *head)
{
    while (head != NULL) {
        StringNode *next = head->next;
        free(head->value);
        free(head);
        head = next;
    }
}

static char *duplicate_range(const char *start, size_t length)
{
    char *copy;

    if (length == SIZE_MAX) {
        errno = ENOMEM;
        return NULL;
    }

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, start, length);
    copy[length] = '\0';
    return copy;
}

static int append_node(StringNode **head, StringNode **tail,
                       const char *start, size_t length)
{
    StringNode *node = malloc(sizeof(*node));

    if (node == NULL) {
        return -1;
    }

    node->value = duplicate_range(start, length);
    if (node->value == NULL) {
        free(node);
        return -1;
    }

    node->next = NULL;

    if (*tail == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    *tail = node;
    return 0;
}

static int string_to_list(const char *input, char delimiter, StringNode **result)
{
    StringNode *head = NULL;
    StringNode *tail = NULL;
    const char *token_start;
    const char *cursor;

    if (input == NULL || result == NULL || delimiter == '\0') {
        errno = EINVAL;
        return -1;
    }

    *result = NULL;
    token_start = input;
    cursor = input;

    for (;;) {
        if (*cursor == delimiter || *cursor == '\0') {
            size_t length = (size_t)(cursor - token_start);

            if (append_node(&head, &tail, token_start, length) != 0) {
                free_list(head);
                return -1;
            }

            if (*cursor == '\0') {
                break;
            }

            token_start = cursor + 1;
        }

        ++cursor;
    }

    *result = head;
    return 0;
}

static int print_list(const StringNode *head)
{
    for (const StringNode *node = head; node != NULL; node = node->next) {
        if (printf("%s\n", node->value) < 0) {
            return -1;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    StringNode *list = NULL;
    char delimiter = ',';

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <string> [delimiter]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 3) {
        if (argv[2][0] == '\0' || argv[2][1] != '\0') {
            fprintf(stderr, "Delimiter must be exactly one character.\n");
            return EXIT_FAILURE;
        }
        delimiter = argv[2][0];
    }

    if (string_to_list(argv[1], delimiter, &list) != 0) {
        perror("string_to_list");
        return EXIT_FAILURE;
    }

    if (print_list(list) != 0) {
        perror("print_list");
        free_list(list);
        return EXIT_FAILURE;
    }

    free_list(list);
    return EXIT_SUCCESS;
}