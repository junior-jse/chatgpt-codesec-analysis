#include <stdio.h>
#include <stdlib.h>

typedef struct NestedList {
    int is_integer;
    int value;
    struct NestedList **list;
    int list_size;
    int list_capacity;
} NestedList;

typedef struct {
    int *data;
    int size;
    int capacity;
} FlatList;

void flat_list_init(FlatList *flat) {
    flat->data = NULL;
    flat->size = 0;
    flat->capacity = 0;
}

void flat_list_free(FlatList *flat) {
    if (flat->data != NULL) {
        free(flat->data);
        flat->data = NULL;
    }
    flat->size = 0;
    flat->capacity = 0;
}

int flat_list_append(FlatList *flat, int value) {
    if (flat->size >= flat->capacity) {
        int new_capacity = (flat->capacity == 0) ? 8 : flat->capacity * 2;
        int *new_data = (int *)realloc(flat->data, new_capacity * sizeof(int));
        if (new_data == NULL) {
            return -1;
        }
        flat->data = new_data;
        flat->capacity = new_capacity;
    }
    flat->data[flat->size] = value;
    flat->size++;
    return 0;
}

NestedList *nested_list_create_int(int value) {
    NestedList *nl = (NestedList *)malloc(sizeof(NestedList));
    if (nl == NULL) {
        return NULL;
    }
    nl->is_integer = 1;
    nl->value = value;
    nl->list = NULL;
    nl->list_size = 0;
    nl->list_capacity = 0;
    return nl;
}

NestedList *nested_list_create_list(void) {
    NestedList *nl = (NestedList *)malloc(sizeof(NestedList));
    if (nl == NULL) {
        return NULL;
    }
    nl->is_integer = 0;
    nl->value = 0;
    nl->list = NULL;
    nl->list_size = 0;
    nl->list_capacity = 0;
    return nl;
}

void nested_list_free(NestedList *nl) {
    if (nl == NULL) {
        return;
    }
    if (!nl->is_integer && nl->list != NULL) {
        for (int i = 0; i < nl->list_size; i++) {
            nested_list_free(nl->list[i]);
        }
        free(nl->list);
    }
    free(nl);
}

int nested_list_add(NestedList *list_nl, NestedList *item) {
    if (list_nl->is_integer) {
        return -1;
    }
    if (list_nl->list_size >= list_nl->list_capacity) {
        int new_capacity = (list_nl->list_capacity == 0) ? 8 : list_nl->list_capacity * 2;
        NestedList **new_list = (NestedList **)realloc(list_nl->list, new_capacity * sizeof(NestedList *));
        if (new_list == NULL) {
            return -1;
        }
        list_nl->list = new_list;
        list_nl->list_capacity = new_capacity;
    }
    list_nl->list[list_nl->list_size] = item;
    list_nl->list_size++;
    return 0;
}

int flatten_recursive(NestedList *nl, FlatList *flat) {
    if (nl == NULL) {
        return -1;
    }
    if (nl->is_integer) {
        return flat_list_append(flat, nl->value);
    }
    for (int i = 0; i < nl->list_size; i++) {
        if (flatten_recursive(nl->list[i], flat) != 0) {
            return -1;
        }
    }
    return 0;
}

FlatList *flatten(NestedList *nested) {
    FlatList *flat = (FlatList *)malloc(sizeof(FlatList));
    if (flat == NULL) {
        return NULL;
    }
    flat_list_init(flat);
    if (flatten_recursive(nested, flat) != 0) {
        flat_list_free(flat);
        free(flat);
        return NULL;
    }
    return flat;
}

int main(void) {
    NestedList *nested = nested_list_create_list();
    NestedList *sublist = nested_list_create_list();
    
    nested_list_add(nested, nested_list_create_int(1));
    nested_list_add(sublist, nested_list_create_int(2));
    nested_list_add(sublist, nested_list_create_int(3));
    nested_list_add(nested, sublist);
    nested_list_add(nested, nested_list_create_int(4));
    
    FlatList *flat = flatten(nested);
    if (flat == NULL) {
        nested_list_free(nested);
        return 1;
    }
    
    for (int i = 0; i < flat->size; i++) {
        printf("%d ", flat->data[i]);
    }
    printf("\n");
    
    flat_list_free(flat);
    free(flat);
    nested_list_free(nested);
    
    return 0;
}