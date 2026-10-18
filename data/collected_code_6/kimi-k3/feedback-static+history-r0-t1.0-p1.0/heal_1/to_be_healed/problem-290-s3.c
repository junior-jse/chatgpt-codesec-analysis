#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t length;
} IntList;

typedef struct {
    IntList *lists;
    size_t count;
} ListCollection;

typedef struct {
    size_t *indices;
    size_t count;
} MaxLengthResult;

IntList* create_int_list(size_t length) {
    IntList *list = malloc(sizeof(IntList));
    if (list == NULL) {
        return NULL;
    }
    
    list->data = malloc(length * sizeof(int));
    if (list->data == NULL) {
        free(list);
        return NULL;
    }
    
    list->length = length;
    return list;
}

void free_int_list(IntList *list) {
    if (list != NULL) {
        free(list->data);
        free(list);
    }
}

ListCollection* create_list_collection(size_t count) {
    ListCollection *collection = malloc(sizeof(ListCollection));
    if (collection == NULL) {
        return NULL;
    }
    
    collection->lists = calloc(count, sizeof(IntList));
    if (collection->lists == NULL) {
        free(collection);
        return NULL;
    }
    
    collection->count = count;
    return collection;
}

void free_list_collection(ListCollection *collection) {
    if (collection != NULL) {
        if (collection->lists != NULL) {
            for (size_t i = 0; i < collection->count; i++) {
                free(collection->lists[i].data);
            }
            free(collection->lists);
        }
        free(collection);
    }
}

MaxLengthResult* find_max_length_lists(const ListCollection *collection) {
    if (collection == NULL || collection->lists == NULL || collection->count == 0) {
        return NULL;
    }
    
    MaxLengthResult *result = malloc(sizeof(MaxLengthResult));
    if (result == NULL) {
        return NULL;
    }
    
    result->indices = malloc(collection->count * sizeof(size_t));
    if (result->indices == NULL) {
        free(result);
        return NULL;
    }
    
    size_t max_length = 0;
    result->count = 0;
    
    for (size_t i = 0; i < collection->count; i++) {
        if (collection->lists[i].length > max_length) {
            max_length = collection->lists[i].length;
        }
    }
    
    for (size_t i = 0; i < collection->count; i++) {
        if (collection->lists[i].length == max_length) {
            result->indices[result->count] = i;
            result->count++;
        }
    }
    
    size_t *temp = realloc(result->indices, result->count * sizeof(size_t));
    if (temp != NULL) {
        result->indices = temp;
    }
    
    return result;
}

void free_max_length_result(MaxLengthResult *result) {
    if (result != NULL) {
        free(result->indices);
        free(result);
    }
}

int main(void) {
    ListCollection *collection = create_list_collection(5);
    if (collection == NULL) {
        fprintf(stderr, "Failed to create list collection\n");
        return EXIT_FAILURE;
    }
    
    /* Possible weaknesses found:
     *  Variable 'sizes' can be declared as const array [constVariable]
     */
    size_t sizes[] = {3, 7, 5, 7, 2};
    
    for (size_t i = 0; i < collection->count; i++) {
        collection->lists[i].data = malloc(sizes[i] * sizeof(int));
        if (collection->lists[i].data == NULL) {
            fprintf(stderr, "Failed to allocate memory for list %zu\n", i);
            free_list_collection(collection);
            return EXIT_FAILURE;
        }
        collection->lists[i].length = sizes[i];
        
        for (size_t j = 0; j < sizes[i]; j++) {
            collection->lists[i].data[j] = (int)(i * 10 + j);
        }
    }
    
    MaxLengthResult *result = find_max_length_lists(collection);
    if (result == NULL) {
        fprintf(stderr, "Failed to find max length lists\n");
        free_list_collection(collection);
        return EXIT_FAILURE;
    }
    
    printf("Lists with maximum length (%zu elements):\n", 
           collection->lists[result->indices[0]].length);
    
    for (size_t i = 0; i < result->count; i++) {
        size_t idx = result->indices[i];
        printf("List %zu: [", idx);
        for (size_t j = 0; j < collection->lists[idx].length; j++) {
            printf("%d", collection->lists[idx].data[j]);
            if (j < collection->lists[idx].length - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
    
    free_max_length_result(result);
    free_list_collection(collection);
    
    return EXIT_SUCCESS;
}