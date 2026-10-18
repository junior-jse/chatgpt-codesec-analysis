#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct KV {
    int key;
    int value;
} KV;

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct DictEntry {
    int key;
    Node *values;
    struct DictEntry *next;
} DictEntry;

typedef struct Dict {
    DictEntry **buckets;
    size_t size;
} Dict;

Dict *dict_create(size_t size) {
    Dict *d = malloc(sizeof(Dict));
    if (!d) return NULL;
    d->buckets = calloc(size, sizeof(DictEntry *));
    if (!d->buckets) {
        free(d);
        return NULL;
    }
    d->size = size;
    return d;
}

void dict_add(Dict *d, int key, int value) {
    if (!d || d->size == 0) return;
    size_t index = (size_t)key % d->size;
    DictEntry *entry = d->buckets[index];
    while (entry) {
        if (entry->key == key) break;
        entry = entry->next;
    }
    if (!entry) {
        entry = malloc(sizeof(DictEntry));
        if (!entry) return;
        entry->key = key;
        entry->values = NULL;
        entry->next = d->buckets[index];
        d->buckets[index] = entry;
    }
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) return;
    new_node->value = value;
    new_node->next = entry->values;
    entry->values = new_node;
}

void dict_free(Dict *d) {
    if (!d) return;
    for (size_t i = 0; i < d->size; i++) {
        DictEntry *entry = d->buckets[i];
        while (entry) {
            Node *n = entry->values;
            while (n) {
                Node *next_n = n->next;
                free(n);
                n = next_n;
            }
            DictEntry *next_e = entry->next;
            free(entry);
            entry = next_e;
        }
    }
    free(d->buckets);
    free(d);
}

Dict *group_kv_pairs(KV *pairs, size_t len, size_t dict_size) {
    if (!pairs || dict_size == 0) return NULL;
    Dict *d = dict_create(dict_size);
    if (!d) return NULL;
    for (size_t i = 0; i < len; i++) {
        dict_add(d, pairs[i].key, pairs[i].value);
    }
    return d;
}

void print_dict(Dict *d) {
    if (!d) return;
    for (size_t i = 0; i < d->size; i++) {
        DictEntry *entry = d->buckets[i];
        while (entry) {
            printf("%d: [", entry->key);
            Node *n = entry->values;
            while (n) {
                printf("%d", n->value);
                if (n->next) printf(", ");
                n = n->next;
            }
            printf("]\n");
            entry = entry->next;
        }
    }
}

int main(void) {
    KV pairs[] = {{1, 10}, {2, 20}, {1, 30}, {3, 40}, {2, 50}, {3, 60}, {1, 70}};
    size_t len = sizeof(pairs) / sizeof(pairs[0]);
    size_t dict_size = 10;
    Dict *d = group_kv_pairs(pairs, len, dict_size);
    if (!d) {
        fprintf(stderr, "Failed to allocate dictionary\n");
        return 1;
    }
    print_dict(d);
    dict_free(d);
    return 0;
}