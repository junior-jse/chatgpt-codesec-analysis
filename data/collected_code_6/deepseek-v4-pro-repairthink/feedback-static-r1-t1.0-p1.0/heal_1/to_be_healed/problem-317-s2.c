#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    int count;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} EncodedList;

Node *create_node(int value, int count) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) return NULL;
    node->value = value;
    node->count = count;
    node->next = NULL;
    return node;
}

void append_node(EncodedList *list, Node *node) {
    if (!list || !node) return;
    if (!list->head) {
        list->head = node;
        return;
    }
    Node *current = list->head;
    while (current->next) {
        current = current->next;
    }
    current->next = node;
}

/* Possible weaknesses found:
 *  Parameter 'input' can be declared as pointer to const [constParameterPointer]
 */
EncodedList *modified_run_length_encode(int *input, int length) {
    if (!input || length <= 0) return NULL;

    EncodedList *encoded = (EncodedList *)malloc(sizeof(EncodedList));
    if (!encoded) return NULL;
    encoded->head = NULL;

    int i = 0;
    while (i < length) {
        int current_value = input[i];
        int count = 1;
        while (i + count < length && input[i + count] == current_value) {
            count++;
        }

        if (count > 1) {
            append_node(encoded, create_node(current_value, count));
            i += count;
        } else {
            int j = i;
            while (j < length && (j + 1 >= length || input[j] != input[j + 1])) {
                j++;
            }
            int single_count = j - i;
            append_node(encoded, create_node(-single_count, 0));
            for (int k = 0; k < single_count; k++) {
                append_node(encoded, create_node(input[i + k], 1));
            }
            if (j < length) {
                int repeat_count = 1;
                while (j + repeat_count < length && input[j + repeat_count] == input[j]) {
                    repeat_count++;
                }
                append_node(encoded, create_node(input[j], repeat_count));
                i = j + repeat_count;
            } else {
                i = j;
            }
        }
    }

    return encoded;
}

void print_encoded_list(EncodedList *list) {
    if (!list || !list->head) return;
    Node *current = list->head;
    while (current) {
        if (current->count == 0) {
            printf("[%d]", current->value);
        } else if (current->count == 1) {
            printf("%d", current->value);
        } else {
            printf("%d,%d", current->value, current->count);
        }
        current = current->next;
        if (current) printf(" ");
    }
    printf("\n");
}

void free_encoded_list(EncodedList *list) {
    if (!list) return;
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int main(void) {
    int test1[] = {1, 1, 2, 3, 3, 3, 4, 4, 4, 4, 5};
    int len1 = sizeof(test1) / sizeof(test1[0]);
    EncodedList *result1 = modified_run_length_encode(test1, len1);
    if (result1) {
        print_encoded_list(result1);
        free_encoded_list(result1);
    }

    int test2[] = {1, 2, 3, 4, 5};
    int len2 = sizeof(test2) / sizeof(test2[0]);
    EncodedList *result2 = modified_run_length_encode(test2, len2);
    if (result2) {
        print_encoded_list(result2);
        free_encoded_list(result2);
    }

    int test3[] = {5, 5, 5, 5, 5};
    int len3 = sizeof(test3) / sizeof(test3[0]);
    EncodedList *result3 = modified_run_length_encode(test3, len3);
    if (result3) {
        print_encoded_list(result3);
        free_encoded_list(result3);
    }

    int test4[] = {1, 1, 2, 2, 3, 3, 4, 4};
    int len4 = sizeof(test4) / sizeof(test4[0]);
    EncodedList *result4 = modified_run_length_encode(test4, len4);
    if (result4) {
        print_encoded_list(result4);
        free_encoded_list(result4);
    }

    return 0;
}