#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DELIM_LEN 256

typedef struct Node {
    char *data;
    struct Node *next;
} Node;

static size_t bounded_strlen(const char *s, size_t maxlen)
{
    size_t len = 0;
    while (len < maxlen && s[len] != '\0') {
        len++;
    }
    return len;
}

static void free_list(Node *head)
{
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

static Node *create_node(const char *start, size_t len)
{
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     *  buf_size is assigned 'len+1' here.
     */
    size_t buf_size = len + 1;
    /* Possible weaknesses found:
     *  Condition 'buf_size<=len' is always false [knownConditionTrueFalse]
     *  Condition 'buf_size<=len' is always false
     *  Assuming condition 'buf_size<=len' is false
     */
    if (buf_size <= len) {
        free(node);
        return NULL;
    }

    node->data = malloc(buf_size);
    /* Possible weaknesses found:
     *  Assuming condition is false
     */
    if (node->data == NULL) {
        free(node);
        return NULL;
    }

    if (len > 0) {
        /* Possible weaknesses found:
         *  Condition 'len>=buf_size' is always false [knownConditionTrueFalse]
         *  Condition 'len>=buf_size' is always false
         */
        if (len >= buf_size) {
            free(node->data);
            free(node);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(node->data, start, len);
    }
    node->data[len] = '\0';
    node->next = NULL;
    return node;
}

Node *string_to_list(const char *str, const char *delim)
{
    if (str == NULL || delim == NULL) {
        return NULL;
    }

    Node *head = NULL;
    Node *tail = NULL;
    const char *start = str;
    const char *p = str;

    size_t delim_len = bounded_strlen(delim, MAX_DELIM_LEN + 1);

    if (delim_len == 0 || delim_len > MAX_DELIM_LEN) {
        return NULL;
    }

    while (*p != '\0') {
        if (strncmp(p, delim, delim_len) == 0) {
            Node *node = create_node(start, (size_t)(p - start));
            if (node == NULL) {
                free_list(head);
                return NULL;
            }

            if (head == NULL) {
                head = node;
            } else {
                tail->next = node;
            }
            tail = node;

            p += delim_len;
            start = p;
        } else {
            p++;
        }
    }

    Node *node = create_node(start, (size_t)(p - start));
    if (node == NULL) {
        free_list(head);
        return NULL;
    }

    if (head == NULL) {
        head = node;
    } else {
        tail->next = node;
    }

    return head;
}

int main(void)
{
    const char *input = "apple,banana,cherry,date";
    Node *list = string_to_list(input, ",");

    if (list == NULL) {
        fprintf(stderr, "Failed to convert string to list\n");
        return EXIT_FAILURE;
    }

    Node *current = list;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }

    free_list(list);
    return EXIT_SUCCESS;
}