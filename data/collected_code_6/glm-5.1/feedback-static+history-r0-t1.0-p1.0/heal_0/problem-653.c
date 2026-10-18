#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *value;
    struct Node *next;
} Node;

typedef struct MapEntry {
    char *key;
    Node *head;
    struct MapEntry *next;
} MapEntry;

typedef struct {
    MapEntry *head;
} Map;

Map *map_create() {
    Map *m = malloc(sizeof(Map));
    if (!m) return NULL;
    m->head = NULL;
    return m;
}

void list_append(Node **head, const char *value) {
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) return;
    
    new_node->value = malloc(strlen(value) + 1);
    if (!new_node->value) {
        free(new_node);
        return;
    }
    strcpy(new_node->value, value);
    new_node->next = NULL;

    if (!*head) {
        *head = new_node;
    } else {
        Node *curr = *head;
        while (curr->next) curr = curr->next;
        curr->next = new_node;
    }
}

void map_insert(Map *m, const char *key, const char *value) {
    if (!m) return;

    MapEntry *entry = m->head;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            list_append(&entry->head, value);
            return;
        }
        entry = entry->next;
    }

    MapEntry *new_entry = malloc(sizeof(MapEntry));
    if (!new_entry) return;

    new_entry->key = malloc(strlen(key) + 1);
    if (!new_entry->key) {
        free(new_entry);
        return;
    }
    strcpy(new_entry->key, key);

    new_entry->head = NULL;
    list_append(&new_entry->head, value);
    new_entry->next = m->head;
    m->head = new_entry;
}

void list_free(Node *head) {
    Node *curr = head;
    while (curr) {
        Node *next = curr->next;
        free(curr->value);
        free(curr);
        curr = next;
    }
}

void map_free(Map *m) {
    if (!m) return;
    MapEntry *curr = m->head;
    while (curr) {
        MapEntry *next = curr->next;
        free(curr->key);
        list_free(curr->head);
        free(curr);
        curr = next;
    }
    free(m);
}

void map_print(const Map *m) {
    if (!m) return;
    MapEntry *entry = m->head;
    while (entry) {
        printf("%s: [", entry->key);
        Node *val = entry->head;
        while (val) {
            printf("\"%s\"", val->value);
            if (val->next) printf(", ");
            val = val->next;
        }
        printf("]\n");
        entry = entry->next;
    }
}

int main() {
    Map *m = map_create();
    if (!m) {
        fprintf(stderr, "Failed to allocate map\n");
        return 1;
    }

    map_insert(m, "fruits", "apple");
    map_insert(m, "colors", "red");
    map_insert(m, "fruits", "banana");
    map_insert(m, "colors", "green");
    map_insert(m, "fruits", "cherry");
    map_insert(m, "animals", "dog");

    map_print(m);

    map_free(m);
    return 0;
}