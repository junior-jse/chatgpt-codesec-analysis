#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *value;
    struct Node *next;
} Node;

typedef struct MapEntry {
    char *key;
    Node *values;
    struct MapEntry *next;
} MapEntry;

typedef struct {
    MapEntry *entries;
} Map;

Map *map_create() {
    Map *map = malloc(sizeof(Map));
    if (!map) return NULL;
    map->entries = NULL;
    return map;
}

void map_add(Map *map, const char *key, const char *value) {
    if (!map || !key || !value) return;
    
    MapEntry *entry = map->entries;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            break;
        }
        entry = entry->next;
    }
    
    if (!entry) {
        entry = malloc(sizeof(MapEntry));
        if (!entry) return;
        entry->key = strdup(key);
        if (!entry->key) { free(entry); return; }
        entry->values = NULL;
        entry->next = map->entries;
        map->entries = entry;
    }
    
    Node *node = malloc(sizeof(Node));
    if (!node) return;
    node->value = strdup(value);
    if (!node->value) { free(node); return; }
    node->next = entry->values;
    entry->values = node;
}

void map_free(Map *map) {
    if (!map) return;
    MapEntry *entry = map->entries;
    while (entry) {
        MapEntry *next_entry = entry->next;
        free(entry->key);
        Node *node = entry->values;
        while (node) {
            Node *next_node = node->next;
            free(node->value);
            free(node);
            node = next_node;
        }
        free(entry);
        entry = next_entry;
    }
    free(map);
}

void map_print(Map *map) {
    if (!map) return;
    MapEntry *entry = map->entries;
    while (entry) {
        printf("%s: [", entry->key);
        Node *node = entry->values;
        while (node) {
            printf("\"%s\"", node->value);
            if (node->next) printf(", ");
            node = node->next;
        }
        printf("]\n");
        entry = entry->next;
    }
}

int main() {
    Map *map = map_create();
    if (!map) return 1;
    
    map_add(map, "fruit", "apple");
    map_add(map, "color", "red");
    map_add(map, "fruit", "banana");
    map_add(map, "color", "blue");
    map_add(map, "fruit", "cherry");
    
    map_print(map);
    map_free(map);
    
    return 0;
}