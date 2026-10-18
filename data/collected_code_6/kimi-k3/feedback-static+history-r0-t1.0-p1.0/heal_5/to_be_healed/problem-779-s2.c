#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

typedef struct {
    int *data;
    size_t size;
} List;

typedef struct {
    List *lists;
    size_t count;
} ListCollection;

int compare_lists(const List *a, const List *b) {
    if (a->size != b->size) {
        return 0;
    }
    if (a->size == 0) {
        return 1;
    }
    if (a->data == NULL || b->data == NULL) {
        return 0;
    }
    return memcmp(a->data, b->data, a->size * sizeof(int)) == 0;
}

size_t count_unique_lists(const ListCollection *collection) {
    if (collection == NULL || collection->lists == NULL || collection->count == 0) {
        return 0;
    }

    size_t unique_count = 0;
    int *processed = calloc(collection->count, sizeof(int));
    if (processed == NULL) {
        return 0;
    }

    for (size_t i = 0; i < collection->count; i++) {
        if (processed[i]) {
            continue;
        }
        unique_count++;
        processed[i] = 1;

        for (size_t j = i + 1; j < collection->count; j++) {
            if (!processed[j] && compare_lists(&collection->lists[i], &collection->lists[j])) {
                processed[j] = 1;
            }
        }
    }

    free(processed);
    return unique_count;
}

void free_collection(ListCollection *collection) {
    if (collection == NULL) {
        return;
    }
    if (collection->lists != NULL) {
        for (size_t i = 0; i < collection->count; i++) {
            free(collection->lists[i].data);
            collection->lists[i].data = NULL;
        }
        free(collection->lists);
        collection->lists = NULL;
    }
    collection->count = 0;
}

int main(void) {
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5};
    int data3[] = {1, 2, 3};
    int data4[] = {6, 7, 8, 9};
    int data5[] = {4, 5};
    int data6[] = {1, 2, 3, 4};

    ListCollection collection = {0};
    collection.count = 6;
    
    if (collection.count > SIZE_MAX / sizeof(List)) {
        fprintf(stderr, "Size overflow detected\n");
        return EXIT_FAILURE;
    }
    
    collection.lists = calloc(collection.count, sizeof(List));
    if (collection.lists == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    const size_t sizes[] = {
        sizeof(data1) / sizeof(data1[0]),
        sizeof(data2) / sizeof(data2[0]),
        sizeof(data3) / sizeof(data3[0]),
        sizeof(data4) / sizeof(data4[0]),
        sizeof(data5) / sizeof(data5[0]),
        sizeof(data6) / sizeof(data6[0])
    };

    const int *const sources[] = {data1, data2, data3, data4, data5, data6};

    for (size_t i = 0; i < collection.count; i++) {
        collection.lists[i].size = sizes[i];
        
        if (sizes[i] > 0) {
            if (sizes[i] > SIZE_MAX / sizeof(int)) {
                fprintf(stderr, "Size overflow detected\n");
                free_collection(&collection);
                return EXIT_FAILURE;
            }
            
            size_t bytes = sizes[i] * sizeof(int);
            
            collection.lists[i].data = malloc(bytes);
            if (collection.lists[i].data == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free_collection(&collection);
                return EXIT_FAILURE;
            }
            
            if (bytes > 0 && sources[i] != NULL) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(collection.lists[i].data, sources[i], bytes);
            } else {
                fprintf(stderr, "Invalid source data\n");
                free_collection(&collection);
                return EXIT_FAILURE;
            }
        } else {
            collection.lists[i].data = NULL;
        }
    }

    size_t unique = count_unique_lists(&collection);
    printf("Number of unique lists: %zu\n", unique);

    free_collection(&collection);
    return EXIT_SUCCESS;
}