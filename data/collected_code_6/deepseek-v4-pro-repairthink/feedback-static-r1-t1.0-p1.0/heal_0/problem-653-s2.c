#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *key;
    int value;
    struct Node *next;
} Node;

typedef struct {
    char *key;
    Node *values;
    struct Entry *next;
} Entry;

typedef struct {
    Entry *head;
    int size;
} Map;

unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

Map *create_map(void) {
    Map *map = (Map *)malloc(sizeof(Map));
    if (!map) return NULL;
    map->head = NULL;
    map->size = 0;
    return map;
}

Entry *find_entry(Map *map, const char *key) {
    Entry *entry = map->head;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

int add_to_entry(Entry *entry, int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) return 0;
    node->value = value;
    node->next = entry->values;
    entry->values = node;
    return 1;
}

int put(Map *map, const char *key, int value) {
    Entry *entry = find_entry(map, key);
    if (entry) {
        return add_to_entry(entry, value);
    }

    entry = (Entry *)malloc(sizeof(Entry));
    if (!entry) return 0;

    entry->key = (char *)malloc(strlen(key) + 1);
    if (!entry->key) {
        free(entry);
        return 0;
    }
    strcpy(entry->key, key);

    entry->values = NULL;
    if (!add_to_entry(entry, value)) {
        free(entry->key);
        free(entry);
        return 0;
    }

    entry->next = map->head;
    map->head = entry;
    map->size++;
    return 1;
}

void free_map(Map *map) {
    Entry *entry = map->head;
    while (entry) {
        Entry *next_entry = entry->next;
        Node *node = entry->values;
        while (node) {
            Node *next_node = node->next;
            free(node);
            node = next_node;
        }
        free(entry->key);
        free(entry);
        entry = next_entry;
    }
    free(map);
}

void print_map(Map *map) {
    Entry *entry = map->head;
    while (entry) {
        printf("%s: [", entry->key);
        Node *node = entry->values;
        while (node) {
            printf("%d", node->value);
            if (node->next) printf(", ");
            node = node->next;
        }
        printf("]\n");
        entry = entry->next;
    }
}

int main(void) {
    Map *map = create_map();
    if (!map) return 1;

    const char *keys[] = {"apple", "banana", "apple", "cherry", "banana", "apple"};
    int values[] = {1, 2, 3, 4, 5, 6};
    int count = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < count; i++) {
        if (!put(map, keys[i], values[i])) {
            free_map(map);
            return 1;
        }
    }

    print_map(map);
    free_map(map);
    return 0;
}