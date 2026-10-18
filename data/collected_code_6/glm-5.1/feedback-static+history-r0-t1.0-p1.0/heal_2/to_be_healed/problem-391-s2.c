#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dict {
    char *key;
    struct Dict *value;
    struct Dict *next;
} Dict;

Dict *create_node(const char *key) {
    Dict *node = malloc(sizeof(Dict));
    if (!node) {
        exit(EXIT_FAILURE);
    }
    size_t len = strnlen(key, SIZE_MAX);
    node->key = malloc(len + 1);
    if (!node->key) {
        free(node);
        exit(EXIT_FAILURE);
    }
    snprintf(node->key, len + 1, "%s", key);
    node->value = NULL;
    node->next = NULL;
    return node;
}

void free_dict(Dict *d) {
    if (!d) return;
    free(d->key);
    free_dict(d->value);
    free_dict(d->next);
    free(d);
}

Dict *list_to_nested_dict(char **lists, int *sizes, int count) {
    if (!lists || count <= 0 || !sizes || sizes[0] <= 0) {
        return NULL;
    }

    Dict *head = NULL;
    Dict *tail = NULL;

    for (int i = 0; i < sizes[0]; i++) {
        /* Possible weaknesses found:
         *  Variable 'key' can be declared as const array [constVariable]
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char key[2] = {lists[0][i], '\0'};
        Dict *node = create_node(key);
        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    if (count > 1) {
        Dict *curr = head;
        while (curr) {
            curr->value = list_to_nested_dict(lists + 1, sizes + 1, count - 1);
            curr = curr->next;
        }
    }

    return head;
}

void print_dict(Dict *d, int depth) {
    if (!d) return;
    Dict *curr = d;
    while (curr) {
        for (int i = 0; i < depth; i++) printf("  ");
        printf("%s", curr->key);
        if (curr->value) {
            printf(":\n");
            print_dict(curr->value, depth + 1);
        } else {
            printf("\n");
        }
        curr = curr->next;
    }
}

int main(void) {
    char list1[] = {'A', 'B', 'C'};
    char list2[] = {'X', 'Y'};
    char list3[] = {'1', '2'};

    char *lists[] = {list1, list2, list3};
    int sizes[] = {3, 2, 2};
    int count = 3;

    Dict *root = list_to_nested_dict(lists, sizes, count);

    print_dict(root, 0);

    free_dict(root);

    return 0;
}