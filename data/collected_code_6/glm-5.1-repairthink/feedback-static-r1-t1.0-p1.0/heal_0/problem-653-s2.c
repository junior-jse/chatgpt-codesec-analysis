#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pair {
    char *key;
    int value;
} Pair;

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct MapEntry {
    char *key;
    Node *head;
    struct MapEntry *next;
} MapEntry;

typedef struct Map {
    MapEntry **buckets;
    size_t size;
} Map;

unsigned long hash_key(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

Map *map_create(size_t size) {
    Map *map = malloc(sizeof(Map));
    if (!map) return NULL;
    map->buckets = calloc(size, sizeof(MapEntry *));
    if (!map->buckets) {
        free(map);
        return NULL;
    }
    map->size = size;
    return map;
}

void map_add(Map *map, const char *key, int value) {
    if (!map || !key) return;
    unsigned long hash = hash_key(key) % map->size;
    MapEntry *entry = map->buckets[hash];
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
        if (!entry->key) {
            free(entry);
            return;
        }
        entry->head = NULL;
        entry->next = map->buckets[hash];
        map->buckets[hash] = entry;
    }
    Node *node = malloc(sizeof(Node));
    if (!node) return;
    node->value = value;
    node->next = entry->head;
    entry->head = node;
}

void map_free(Map *map) {
    if (!map) return;
    for (size_t i = 0; i < map->size; i++) {
        MapEntry *entry = map->buckets[i];
        while (entry) {
            MapEntry *next_entry = entry->next;
            Node *node = entry->head;
            while (node) {
                Node *next_node = node->next;
                free(node);
                node = next_node;
            }
            free(entry->key);
            free(entry);
            entry = next_entry;
        }
    }
    free(map->buckets);
    free(map);
}

Map *group_pairs(Pair *pairs, size_t count) {
    Map *map = map_create(64);
    if (!map) return NULL;
    for (size_t i = 0; i < count; i++) {
        map_add(map, pairs[i].key, pairs[i].value);
    }
    return map;
}

void print_map(Map *map) {
    if (!map) return;
    for (size_t i = 0; i < map->size; i++) {
        MapEntry *entry = map->buckets[i];
        while (entry) {
            printf("%s: [", entry->key);
            Node *node = entry->head;
            while (node) {
                printf("%d", node->value);
                if (node->next) printf(", ");
                node = node->next;
            }
            printf("]\n");
            entry = entry->next;
        }
    }
}

int main(void) {
    Pair pairs[] = {
        {"fruit", 1}, {"color", 3}, {"fruit", 2},
        {"color", 4}, {"fruit", 5}, {"shape", 6}
    };
    size_t count = sizeof(pairs) / sizeof(pairs[0]);
    Map *map = group_pairs(pairs, count);
    if (map) {
        print_map(map);
        map_free(map);
    }
    return 0;
}