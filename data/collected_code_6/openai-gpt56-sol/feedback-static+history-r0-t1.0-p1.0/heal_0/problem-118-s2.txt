#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct ListNode {
    char *value;
    struct ListNode *next;
} ListNode;

static void free_list(ListNode *head)
{
    while (head != NULL) {
        ListNode *next = head->next;
        free(head->value);
        free(head);
        head = next;
    }
}

static ListNode *string_to_list(const char *input, const char *delimiter)
{
    ListNode *head = NULL;
    ListNode *tail = NULL;
    char *copy = NULL;
    char *token = NULL;
    char *saveptr = NULL;

    if (input == NULL || delimiter == NULL || delimiter[0] == '\0') {
        errno = EINVAL;
        return NULL;
    }

    copy = strdup(input);
    if (copy == NULL) {
        return NULL;
    }

    token = strtok_r(copy, delimiter, &saveptr);
    while (token != NULL) {
        ListNode *node = calloc(1, sizeof(*node));

        if (node == NULL) {
            free(copy);
            free_list(head);
            return NULL;
        }

        node->value = strdup(token);
        if (node->value == NULL) {
            free(node);
            free(copy);
            free_list(head);
            return NULL;
        }

        if (tail == NULL) {
            head = node;
        } else {
            tail->next = node;
        }
        tail = node;
        token = strtok_r(NULL, delimiter, &saveptr);
    }

    free(copy);
    return head;
}

static void print_list(const ListNode *head)
{
    for (const ListNode *node = head; node != NULL; node = node->next) {
        if (printf("%s\n", node->value) < 0) {
            return;
        }
    }
}

int main(int argc, char *argv[])
{
    ListNode *list = NULL;

    if (argc != 3) {
        if (fprintf(stderr, "Usage: %s <string> <delimiters>\n", argv[0]) < 0) {
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    errno = 0;
    list = string_to_list(argv[1], argv[2]);
    if (list == NULL) {
        if (errno != 0) {
            perror("string_to_list");
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    print_list(list);

    if (ferror(stdout)) {
        free_list(list);
        return EXIT_FAILURE;
    }

    free_list(list);
    return EXIT_SUCCESS;
}