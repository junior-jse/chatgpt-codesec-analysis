#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
    size_t chunk_size;
} TupleChunk;

typedef struct {
    TupleChunk *chunks;
    size_t count;
    size_t capacity;
} ChunkList;

void init_chunk_list(ChunkList *list) {
    list->chunks = NULL;
    list->count = 0;
    list->capacity = 0;
}

void free_chunk_list(ChunkList *list) {
    size_t i;
    if (list == NULL) {
        return;
    }
    for (i = 0; i < list->count; i++) {
        if (list->chunks[i].tuples != NULL) {
            size_t j;
            for (j = 0; j < list->chunks[i].count; j++) {
                free(list->chunks[i].tuples[j].data);
                list->chunks[i].tuples[j].data = NULL;
            }
            free(list->chunks[i].tuples);
            list->chunks[i].tuples = NULL;
        }
    }
    free(list->chunks);
    list->chunks = NULL;
    list->count = 0;
    list->capacity = 0;
}

int add_chunk(ChunkList *list, TupleChunk chunk) {
    TupleChunk *new_chunks;
    
    if (list->count == list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 8 : list->capacity * 2;
        if (new_capacity > (size_t)(-1) / sizeof(TupleChunk)) {
            return 0;
        }
        new_chunks = (TupleChunk *)realloc(list->chunks, new_capacity * sizeof(TupleChunk));
        if (new_chunks == NULL) {
            return 0;
        }
        list->chunks = new_chunks;
        list->capacity = new_capacity;
    }
    
    list->chunks[list->count] = chunk;
    list->count++;
    return 1;
}

static void free_chunk_tuples_data(TupleChunk *chunk) {
    size_t i;
    if (chunk == NULL || chunk->tuples == NULL) {
        return;
    }
    for (i = 0; i < chunk->count; i++) {
        free(chunk->tuples[i].data);
        chunk->tuples[i].data = NULL;
    }
}

ChunkList chunk_tuples(const Tuple *tuples, size_t tuple_count, size_t n) {
    ChunkList result;
    size_t i;
    size_t current_chunk_size = 0;
    TupleChunk current_chunk;
    
    init_chunk_list(&result);
    
    if (tuples == NULL || n == 0 || tuple_count == 0) {
        return result;
    }
    
    if (n > (size_t)(-1) / sizeof(Tuple)) {
        return result;
    }
    
    if (tuple_count > (size_t)(-1) / sizeof(Tuple)) {
        return result;
    }
    
    current_chunk.tuples = (Tuple *)malloc(n * sizeof(Tuple));
    if (current_chunk.tuples == NULL) {
        return result;
    }
    current_chunk.chunk_size = n;
    current_chunk.count = 0;
    
    for (i = 0; i < tuple_count; i++) {
        if (current_chunk_size >= n) {
            if (!add_chunk(&result, current_chunk)) {
                free_chunk_tuples_data(&current_chunk);
                free(current_chunk.tuples);
                free_chunk_list(&result);
                init_chunk_list(&result);
                return result;
            }
            current_chunk.tuples = (Tuple *)malloc(n * sizeof(Tuple));
            if (current_chunk.tuples == NULL) {
                free_chunk_list(&result);
                init_chunk_list(&result);
                return result;
            }
            current_chunk.chunk_size = n;
            current_chunk.count = 0;
            current_chunk_size = 0;
        }
        
        if (current_chunk_size < n) {
            Tuple *dest = &current_chunk.tuples[current_chunk_size];
            if (tuples[i].size > (size_t)(-1) / sizeof(int)) {
                free_chunk_tuples_data(&current_chunk);
                free(current_chunk.tuples);
                free_chunk_list(&result);
                init_chunk_list(&result);
                return result;
            }
            dest->data = (int *)malloc(tuples[i].size * sizeof(int));
            if (dest->data == NULL) {
                free_chunk_tuples_data(&current_chunk);
                free(current_chunk.tuples);
                free_chunk_list(&result);
                init_chunk_list(&result);
                return result;
            }
            if (tuples[i].size > 0 && tuples[i].data != NULL) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(dest->data, tuples[i].data, tuples[i].size * sizeof(int));
            }
            dest->size = tuples[i].size;
            current_chunk.count++;
            current_chunk_size++;
        }
    }
    
    if (current_chunk_size > 0) {
        Tuple *shrunk_tuples = (Tuple *)realloc(current_chunk.tuples, current_chunk_size * sizeof(Tuple));
        if (shrunk_tuples != NULL) {
            current_chunk.tuples = shrunk_tuples;
        }
        current_chunk.chunk_size = current_chunk_size;
        if (!add_chunk(&result, current_chunk)) {
            free_chunk_tuples_data(&current_chunk);
            free(current_chunk.tuples);
            free_chunk_list(&result);
            init_chunk_list(&result);
            return result;
        }
    } else {
        free(current_chunk.tuples);
    }
    
    return result;
}

Tuple *create_tuple(const int *data, size_t size) {
    Tuple *tuple;
    int *new_data;
    
    if (data == NULL || size == 0) {
        return NULL;
    }
    
    if (size > (size_t)(-1) / sizeof(int)) {
        return NULL;
    }
    
    tuple = (Tuple *)malloc(sizeof(Tuple));
    if (tuple == NULL) {
        return NULL;
    }
    
    new_data = (int *)malloc(size * sizeof(int));
    if (new_data == NULL) {
        free(tuple);
        return NULL;
    }
    
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(new_data, data, size * sizeof(int));
    tuple->data = new_data;
    tuple->size = size;
    return tuple;
}

void free_tuples(Tuple *tuples, size_t count) {
    size_t i;
    if (tuples == NULL) {
        return;
    }
    for (i = 0; i < count; i++) {
        free(tuples[i].data);
        tuples[i].data = NULL;
        tuples[i].size = 0;
    }
    free(tuples);
}

int main(void) {
    /* Possible weaknesses found:
     *  Shadowed declaration
     */
    size_t i, j, k;
    Tuple *tuples;
    int data[][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
        {10, 11, 12},
        {13, 14, 15}
    };
    size_t tuple_count = sizeof(data) / sizeof(data[0]);
    size_t chunk_size = 2;
    ChunkList chunks;
    
    if (tuple_count > (size_t)(-1) / sizeof(Tuple)) {
        return 1;
    }
    
    tuples = (Tuple *)malloc(tuple_count * sizeof(Tuple));
    if (tuples == NULL) {
        return 1;
    }
    
    for (i = 0; i < tuple_count; i++) {
        Tuple *t = create_tuple(data[i], 3);
        if (t == NULL) {
            /* Possible weaknesses found:
             *  Local variable 'k' shadows outer variable [shadowVariable]
             *  Shadow variable
             */
            size_t k;
            for (k = 0; k < i; k++) {
                free(tuples[k].data);
                tuples[k].data = NULL;
                tuples[k].size = 0;
            }
            free(tuples);
            return 1;
        }
        tuples[i] = *t;
        free(t);
    }
    
    chunks = chunk_tuples(tuples, tuple_count, chunk_size);
    
    printf("Number of chunks: %zu\n", chunks.count);
    for (i = 0; i < chunks.count; i++) {
        printf("Chunk %zu (size: %zu):\n", i, chunks.chunks[i].chunk_size);
        for (j = 0; j < chunks.chunks[i].count; j++) {
            printf("  Tuple %zu: [", j);
            for (k = 0; k < chunks.chunks[i].tuples[j].size; k++) {
                printf("%d", chunks.chunks[i].tuples[j].data[k]);
                if (k < chunks.chunks[i].tuples[j].size - 1) {
                    printf(", ");
                }
            }
            printf("]\n");
        }
    }
    
    free_tuples(tuples, tuple_count);
    free_chunk_list(&chunks);
    
    return 0;
}