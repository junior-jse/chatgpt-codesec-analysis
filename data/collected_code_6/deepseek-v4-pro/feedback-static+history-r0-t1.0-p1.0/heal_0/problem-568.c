#include <stdio.h>
#include <stdlib.h>

typedef struct Dictionary {
    int placeholder;
} Dictionary;

typedef struct DictNode {
    Dictionary *dict;
    struct DictNode *next;
} DictNode;

typedef struct DictList {
    DictNode *head;
    size_t size;
} DictList;

DictList *create_empty_dict_list(size_t count) {
    DictList *list = malloc(sizeof(DictList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->size = 0;

    for (size_t i = 0; i < count; i++) {
        Dictionary *dict = malloc(sizeof(Dictionary));
        if (dict == NULL) {
            DictNode *current = list->head;
            while (current != NULL) {
                DictNode *next = current->next;
                free(current->dict);
                free(current);
                current = next;
            }
            free(list);
            return NULL;
        }

        DictNode *node = malloc(sizeof(DictNode));
        if (node == NULL) {
            free(dict);
            DictNode *current = list->head;
            while (current != NULL) {
                DictNode *next = current->next;
                free(current->dict);
                free(current);
                current = next;
            }
            free(list);
            return NULL;
        }

        dict->placeholder = 0;
        node->dict = dict;
        node->next = list->head;
        list->head = node;
        list->size++;
    }

    return list;
}

void free_dict_list(DictList *list) {
    if (list == NULL) {
        return;
    }
    DictNode *current = list->head;
    while (current != NULL) {
        DictNode *next = current->next;
        free(current->dict);
        free(current);
        current = next;
    }
    free(list);
}

int main(void) {
    DictList *list = create_empty_dict_list(5);
    if (list == NULL) {
        fprintf(stderr, "Failed to create dictionary list\n");
        return EXIT_FAILURE;
    }

    printf("Created list with %zu dictionaries\n", list->size);

    free_dict_list(list);
    return EXIT_SUCCESS;
}