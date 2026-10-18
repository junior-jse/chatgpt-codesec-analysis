#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dict_entry {
    char *key;
    char *value;
    struct dict_entry *next;
} dict_entry_t;

typedef struct dict {
    dict_entry_t *head;
} dict_t;

dict_t *dict_create(void) {
    dict_t *d = malloc(sizeof(*d));
    if (!d) return NULL;
    d->head = NULL;
    return d;
}

int dict_set(dict_t *d, const char *key, const char *value) {
    dict_entry_t *e;
    for (e = d->head; e; e = e->next) {
        if (strcmp(e->key, key) == 0) {
            char *nv = strdup(value);
            if (!nv) return 0;
            free(e->value);
            e->value = nv;
            return 1;
        }
    }
    e = malloc(sizeof(*e));
    if (!e) return 0;
    e->key = strdup(key);
    e->value = strdup(value);
    if (!e->key || !e->value) {
        free(e->key);
        free(e->value);
        free(e);
        return 0;
    }
    e->next = d->head;
    d->head = e;
    return 1;
}

dict_t *dict_merge(const dict_t *a, const dict_t *b) {
    dict_t *m = dict_create();
    if (!m) return NULL;
    const dict_entry_t *e;
    for (e = a->head; e; e = e->next) {
        if (!dict_set(m, e->key, e->value)) {
            dict_destroy(m);
            return NULL;
        }
    }
    for (e = b->head; e; e = e->next) {
        if (!dict_set(m, e->key, e->value)) {
            dict_destroy(m);
            return NULL;
        }
    }
    return m;
}

void dict_destroy(dict_t *d) {
    if (!d) return;
    dict_entry_t *e = d->head;
    while (e) {
        dict_entry_t *next = e->next;
        free(e->key);
        free(e->value);
        free(e);
        e = next;
    }
    free(d);
}

void dict_print(const dict_t *d) {
    for (const dict_entry_t *e = d->head; e; e = e->next) {
        printf("%s: %s\n", e->key, e->value);
    }
}

int main(void) {
    dict_t *a = dict_create();
    dict_t *b = dict_create();
    if (!a || !b) {
        dict_destroy(a);
        dict_destroy(b);
        return 1;
    }

    if (!dict_set(a, "name", "Alice") ||
        !dict_set(a, "age", "30") ||
        !dict_set(b, "city", "Paris") ||
        !dict_set(b, "age", "31")) {
        dict_destroy(a);
        dict_destroy(b);
        return 1;
    }

    dict_t *merged = dict_merge(a, b);
    if (!merged) {
        dict_destroy(a);
        dict_destroy(b);
        return 1;
    }

    dict_print(merged);

    dict_destroy(merged);
    dict_destroy(a);
    dict_destroy(b);
    return 0;
}