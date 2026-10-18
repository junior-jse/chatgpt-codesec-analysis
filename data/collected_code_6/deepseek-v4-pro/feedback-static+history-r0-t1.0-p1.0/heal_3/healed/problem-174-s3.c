#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *key;
    char *value;
    struct Node *next;
} Node;

typedef struct {
    Node **buckets;
    int num_buckets;
} Dictionary;

unsigned int hash(const char *key, int num_buckets) {
    unsigned int hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % num_buckets;
}

Dictionary *create_dictionary(int num_buckets) {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    dict->num_buckets = num_buckets;
    dict->buckets = (Node **)calloc(num_buckets, sizeof(Node *));
    if (dict->buckets == NULL) {
        free(dict);
        return NULL;
    }
    return dict;
}

int insert_pair(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }

    size_t key_len = strnlen(key, 1024);
    size_t value_len = strnlen(value, 1024);

    if (key_len == 1024 || value_len == 1024) {
        return -1;
    }

    unsigned int index = hash(key, dict->num_buckets);
    Node *current = dict->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            char *new_value = (char *)malloc(value_len + 1);
            if (new_value == NULL) {
                return -1;
            }
            memcpy(new_value, value, value_len);
            new_value[value_len] = '\0';
            free(current->value);
            current->value = new_value;
            return 0;
        }
        current = current->next;
    }
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return -1;
    }
    new_node->key = (char *)malloc(key_len + 1);
    new_node->value = (char *)malloc(value_len + 1);
    if (new_node->key == NULL || new_node->value == NULL) {
        free(new_node->key);
        free(new_node->value);
        free(new_node);
        return -1;
    }
    memcpy(new_node->key, key, key_len);
    new_node->key[key_len] = '\0';
    memcpy(new_node->value, value, value_len);
    new_node->value[value_len] = '\0';
    new_node->next = dict->buckets[index];
    dict->buckets[index] = new_node;
    return 0;
}

void free_dictionary(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    for (int i = 0; i < dict->num_buckets; i++) {
        Node *current = dict->buckets[i];
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(dict->buckets);
    free(dict);
}

void print_dictionary(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    for (int i = 0; i < dict->num_buckets; i++) {
        Node *current = dict->buckets[i];
        while (current != NULL) {
            printf("Key: %s, Value: %s\n", current->key, current->value);
            current = current->next;
        }
    }
}

int main(void) {
    Dictionary *dict = create_dictionary(10);
    if (dict == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return 1;
    }

    insert_pair(dict, "key1", "value1");
    insert_pair(dict, "key2", "value2");
    insert_pair(dict, "key1", "updated_value1");
    insert_pair(dict, "key3", "value3");

    print_dictionary(dict);
    free_dictionary(dict);

    return 0;
}