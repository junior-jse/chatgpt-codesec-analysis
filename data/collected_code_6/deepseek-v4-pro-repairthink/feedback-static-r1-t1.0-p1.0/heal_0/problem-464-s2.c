#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dict_entry {
    char *key;
    int value;
    struct dict_entry *next;
} dict_entry;

typedef struct dictionary {
    dict_entry *head;
    dict_entry *tail;
    int size;
} dictionary;

dictionary *create_dictionary(void) {
    dictionary *d = (dictionary *)malloc(sizeof(dictionary));
    if (d == NULL) {
        return NULL;
    }
    d->head = NULL;
    d->tail = NULL;
    d->size = 0;
    return d;
}

int dict_add(dictionary *d, const char *key, int value) {
    if (d == NULL || key == NULL) {
        return -1;
    }

    dict_entry *entry = (dict_entry *)malloc(sizeof(dict_entry));
    if (entry == NULL) {
        return -1;
    }

    entry->key = (char *)malloc(strlen(key) + 1);
    if (entry->key == NULL) {
        free(entry);
        return -1;
    }
    strcpy(entry->key, key);
    entry->value = value;
    entry->next = NULL;

    if (d->tail == NULL) {
        d->head = entry;
        d->tail = entry;
    } else {
        d->tail->next = entry;
        d->tail = entry;
    }
    d->size++;
    return 0;
}

void destroy_dictionary(dictionary *d) {
    if (d == NULL) {
        return;
    }
    dict_entry *current = d->head;
    while (current != NULL) {
        dict_entry *next = current->next;
        free(current->key);
        free(current);
        current = next;
    }
    free(d);
}

int all_values_same(dictionary *d) {
    if (d == NULL) {
        return -1;
    }
    if (d->size == 0) {
        return 1;
    }

    int first_value = d->head->value;
    dict_entry *current = d->head->next;

    while (current != NULL) {
        if (current->value != first_value) {
            return 0;
        }
        current = current->next;
    }

    return 1;
}

int main(void) {
    dictionary *d = create_dictionary();
    if (d == NULL) {
        return 1;
    }

    if (dict_add(d, "one", 5) != 0) {
        destroy_dictionary(d);
        return 1;
    }
    if (dict_add(d, "two", 5) != 0) {
        destroy_dictionary(d);
        return 1;
    }
    if (dict_add(d, "three", 5) != 0) {
        destroy_dictionary(d);
        return 1;
    }

    int result = all_values_same(d);
    printf("All values same: %s\n", result == 1 ? "yes" : result == 0 ? "no" : "error");

    if (dict_add(d, "four", 10) != 0) {
        destroy_dictionary(d);
        return 1;
    }

    result = all_values_same(d);
    printf("All values same: %s\n", result == 1 ? "yes" : result == 0 ? "no" : "error");

    destroy_dictionary(d);

    dictionary *empty_d = create_dictionary();
    if (empty_d == NULL) {
        return 1;
    }

    result = all_values_same(empty_d);
    printf("All values same in empty dict: %s\n", result == 1 ? "yes" : result == 0 ? "no" : "error");

    destroy_dictionary(empty_d);

    return 0;
}