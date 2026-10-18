#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    int key;
    Node *head;
} Group;

typedef struct {
    Group *entries;
    int size;
    int capacity;
} GroupMap;

GroupMap *create_map(int capacity) {
    GroupMap *map = (GroupMap *)malloc(sizeof(GroupMap));
    map->entries = (Group *)calloc(capacity, sizeof(Group));
    map->size = 0;
    map->capacity = capacity;
    return map;
}

void insert_map(GroupMap *map, int key, int value) {
    for (int i = 0; i < map->size; i++) {
        if (map->entries[i].key == key) {
            Node *new_node = (Node *)malloc(sizeof(Node));
            new_node->value = value;
            new_node->next = map->entries[i].head;
            map->entries[i].head = new_node;
            return;
        }
    }
    if (map->size == map->capacity) {
        map->capacity *= 2;
        map->entries = (Group *)realloc(map->entries, map->capacity * sizeof(Group));
        memset(map->entries + map->size, 0, (map->capacity - map->size) * sizeof(Group));
    }
    map->entries[map->size].key = key;
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = NULL;
    map->entries[map->size].head = new_node;
    map->size++;
}

void free_map(GroupMap *map) {
    for (int i = 0; i < map->size; i++) {
        Node *current = map->entries[i].head;
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(map->entries);
    free(map);
}

void group_by_second(Tuple *tuples, int count) {
    GroupMap *map = create_map(count > 0 ? count : 4);
    for (int i = 0; i < count; i++) {
        insert_map(map, tuples[i].second, tuples[i].first);
    }
    for (int i = 0; i < map->size; i++) {
        printf("%d: [", map->entries[i].key);
        Node *current = map->entries[i].head;
        while (current != NULL) {
            printf("%d", current->value);
            if (current->next != NULL) {
                printf(", ");
            }
            current = current->next;
        }
        printf("]\n");
    }
    free_map(map);
}

int main(void) {
    Tuple data[] = {{1, 2}, {3, 1}, {5, 2}, {7, 3}, {9, 1}};
    int count = sizeof(data) / sizeof(data[0]);
    group_by_second(data, count);
    return 0;
}