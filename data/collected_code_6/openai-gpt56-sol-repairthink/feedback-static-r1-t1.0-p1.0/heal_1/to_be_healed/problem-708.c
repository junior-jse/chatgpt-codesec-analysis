#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct ListNode {
    char *value;
    struct ListNode *next;
} ListNode;

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

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, start, length);
    copy[length] = '\0';

    return copy;
}

static void free_list(ListNode *head)
{
    while (head != NULL) {
        ListNode *next = head->next;
        free(head->value);
        free(head);
        head = next;
    }
}

static int append_node(ListNode **head, ListNode **tail,
                       const char *start, size_t length)
{
    ListNode *node = malloc(sizeof(*node));

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

static int string_to_list(const char *input, char delimiter, ListNode **result)
{
    ListNode *head = NULL;
    ListNode *tail = NULL;
    const char *token_start;
    const char *current;

    if (input == NULL || result == NULL || delimiter == '\0') {
        errno = EINVAL;
        return -1;
    }

    *result = NULL;
    token_start = input;
    current = input;

    for (;;) {
        if (*current == delimiter || *current == '\0') {
            size_t length = (size_t)(current - token_start);

            if (append_node(&head, &tail, token_start, length) != 0) {
                free_list(head);
                errno = ENOMEM;
                return -1;
            }

            if (*current == '\0') {
                break;
            }

            token_start = current + 1;
        }

        ++current;
    }

    *result = head;
    return 0;
}

static int print_list(const ListNode *head)
{
    for (const ListNode *node = head; node != NULL; node = node->next) {
        if (printf("%s\n", node->value) < 0) {
            return -1;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    ListNode *list = NULL;

    if (argc != 3 || argv[2][0] == '\0' || argv[2][1] != '\0') {
        fprintf(stderr, "Usage: %s <string> <delimiter>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (string_to_list(argv[1], argv[2][0], &list) != 0) {
        perror("string_to_list");
        return EXIT_FAILURE;
    }

    if (print_list(list) != 0) {
        free_list(list);
        fprintf(stderr, "Failed to write output\n");
        return EXIT_FAILURE;
    }

    free_list(list);
    return EXIT_SUCCESS;
}