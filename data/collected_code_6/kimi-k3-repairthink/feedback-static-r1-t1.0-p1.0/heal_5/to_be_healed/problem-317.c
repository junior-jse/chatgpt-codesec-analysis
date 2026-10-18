#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

typedef struct {
    void *value;
    size_t count;
} RLEPair;

typedef struct {
    RLEPair *pairs;
    size_t value_size;
    size_t length;
    size_t capacity;
} RLEList;

RLEList* rle_create(size_t value_size, size_t initial_capacity) {
    if (value_size == 0 || initial_capacity == 0) {
        return NULL;
    }
    if (initial_capacity > SIZE_MAX / sizeof(RLEPair)) {
        return NULL;
    }
    RLEList *list = malloc(sizeof(RLEList));
    if (list == NULL) {
        return NULL;
    }
    list->pairs = calloc(initial_capacity, sizeof(RLEPair));
    if (list->pairs == NULL) {
        free(list);
        return NULL;
    }
    list->value_size = value_size;
    list->length = 0;
    list->capacity = initial_capacity;
    return list;
}

void rle_destroy(RLEList *list) {
    if (list == NULL) {
        return;
    }
    if (list->pairs != NULL) {
        for (size_t i = 0; i < list->length; i++) {
            free(list->pairs[i].value);
            list->pairs[i].value = NULL;
        }
        free(list->pairs);
        list->pairs = NULL;
    }
    free(list);
}

int rle_append(RLEList *list, const void *value, size_t count) {
    if (list == NULL || value == NULL || count == 0) {
        return -1;
    }
    if (list->pairs == NULL || list->value_size == 0) {
        return -1;
    }
    if (list->length > list->capacity) {
        return -1;
    }
    if (list->length > 0) {
        RLEPair *last = &list->pairs[list->length - 1];
        if (last->value != NULL && memcmp(last->value, value, list->value_size) == 0) {
            if (count > SIZE_MAX - last->count) {
                return -1;
            }
            last->count += count;
            return 0;
        }
    }
    if (list->length >= list->capacity) {
        if (list->capacity == 0 || list->capacity > SIZE_MAX / (2 * sizeof(RLEPair))) {
            return -1;
        }
        size_t new_capacity = list->capacity * 2;
        if (new_capacity <= list->capacity || new_capacity > SIZE_MAX / sizeof(RLEPair)) {
            return -1;
        }
        RLEPair *new_pairs = realloc(list->pairs, new_capacity * sizeof(RLEPair));
        if (new_pairs == NULL) {
            return -1;
        }
        list->pairs = new_pairs;
        list->capacity = new_capacity;
    }
    size_t value_size = list->value_size;
    if (value_size == 0 || value_size > SIZE_MAX / 2) {
        return -1;
    }
    void *value_copy = malloc(value_size);
    if (value_copy == NULL) {
        return -1;
    }
    unsigned char *dst = (unsigned char *)value_copy;
    const unsigned char *src = (const unsigned char *)value;
    /* Possible weaknesses found:
     *  'copy_size' is assigned value 'value_size' here.
     */
    size_t copy_size = value_size;
    /* Possible weaknesses found:
     *  The comparison 'copy_size > value_size' is always false because 'copy_size' and 'value_size' represent the same value. [knownConditionTrueFalse]
     *  The comparison 'copy_size > value_size' is always false because 'copy_size' and 'value_size' represent the same value.
     */
    if (copy_size > value_size) {
        free(value_copy);
        return -1;
    }
    for (size_t i = 0; i < copy_size; i++) {
        dst[i] = src[i];
    }
    list->pairs[list->length].value = value_copy;
    list->pairs[list->length].count = count;
    list->length++;
    return 0;
}

void rle_free_values(RLEList *list) {
    if (list == NULL) {
        return;
    }
    if (list->pairs == NULL) {
        return;
    }
    for (size_t i = 0; i < list->length; i++) {
        free(list->pairs[i].value);
        list->pairs[i].value = NULL;
    }
}

int* decode_rle(const int *encoded, size_t encoded_len, size_t *decoded_len) {
    if (encoded == NULL || decoded_len == NULL || encoded_len == 0) {
        return NULL;
    }
    if (encoded_len % 2 != 0) {
        return NULL;
    }
    if (encoded_len > SIZE_MAX - 1) {
        return NULL;
    }
    size_t total = 0;
    for (size_t i = 0; i < encoded_len; i += 2) {
        if (encoded[i] <= 0) {
            return NULL;
        }
        size_t count = (size_t)encoded[i];
        if (count > SIZE_MAX - total) {
            return NULL;
        }
        total += count;
        if (total > SIZE_MAX / sizeof(int)) {
            return NULL;
        }
    }
    if (total == 0) {
        return NULL;
    }
    int *result = malloc(total * sizeof(int));
    if (result == NULL) {
        return NULL;
    }
    size_t idx = 0;
    for (size_t i = 0; i < encoded_len && idx < total; i += 2) {
        int count = encoded[i];
        int value = encoded[i + 1];
        for (int j = 0; j < count && idx < total; j++) {
            result[idx++] = value;
        }
    }
    if (idx != total) {
        free(result);
        return NULL;
    }
    *decoded_len = total;
    return result;
}

int* encode_rle(const int *data, size_t data_len, size_t *encoded_len) {
    if (data == NULL || encoded_len == NULL || data_len == 0) {
        return NULL;
    }
    if (data_len > SIZE_MAX / (2 * sizeof(int))) {
        return NULL;
    }
    size_t max_elems = data_len * 2;
    if (max_elems > SIZE_MAX / sizeof(int)) {
        return NULL;
    }
    int *result = malloc(max_elems * sizeof(int));
    if (result == NULL) {
        return NULL;
    }
    size_t out_idx = 0;
    size_t i = 0;
    while (i < data_len) {
        int current = data[i];
        size_t run = 1;
        while (i + run < data_len && data[i + run] == current) {
            run++;
        }
        size_t remaining = run;
        while (remaining > 0) {
            size_t chunk = remaining;
            if (chunk > (size_t)INT_MAX) {
                chunk = (size_t)INT_MAX;
            }
            if (out_idx > max_elems - 2) {
                free(result);
                return NULL;
            }
            result[out_idx++] = (int)chunk;
            result[out_idx++] = current;
            remaining -= chunk;
        }
        i += run;
    }
    *encoded_len = out_idx;
    return result;
}

int main(void) {
    int data[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
    size_t data_len = sizeof(data) / sizeof(data[0]);
    size_t encoded_len = 0;
    size_t decoded_len = 0;

    int *encoded = encode_rle(data, data_len, &encoded_len);
    if (encoded == NULL) {
        fprintf(stderr, "Encoding failed\n");
        return EXIT_FAILURE;
    }

    printf("Original: ");
    for (size_t i = 0; i < data_len; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    printf("Encoded: ");
    for (size_t i = 0; i < encoded_len; i++) {
        printf("%d ", encoded[i]);
    }
    printf("\n");

    int *decoded = decode_rle(encoded, encoded_len, &decoded_len);
    if (decoded == NULL) {
        fprintf(stderr, "Decoding failed\n");
        free(encoded);
        return EXIT_FAILURE;
    }

    printf("Decoded: ");
    for (size_t i = 0; i < decoded_len; i++) {
        printf("%d ", decoded[i]);
    }
    printf("\n");

    if (decoded_len == data_len && data_len <= SIZE_MAX / sizeof(int) &&
        memcmp(data, decoded, data_len * sizeof(int)) == 0) {
        printf("Verification: PASSED\n");
    } else {
        printf("Verification: FAILED\n");
    }

    free(encoded);
    free(decoded);

    RLEList *list = rle_create(sizeof(int), 4);
    if (list == NULL) {
        fprintf(stderr, "RLE list creation failed\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < data_len; i++) {
        if (rle_append(list, &data[i], 1) != 0) {
            fprintf(stderr, "RLE append failed\n");
            rle_destroy(list);
            return EXIT_FAILURE;
        }
    }

    printf("RLEList pairs: ");
    for (size_t i = 0; i < list->length; i++) {
        if (list->pairs[i].value != NULL) {
            printf("(%d, %zu) ", *(int*)list->pairs[i].value, list->pairs[i].count);
        }
    }
    printf("\n");

    rle_destroy(list);

    return EXIT_SUCCESS;
}