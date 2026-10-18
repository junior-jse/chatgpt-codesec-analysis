#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int count;
    int value;
} RLEPair;

typedef struct {
    RLEPair *pairs;
    size_t length;
    size_t capacity;
} RLEList;

RLEList* rle_encode(const int *data, size_t data_len) {
    RLEList *list = malloc(sizeof(RLEList));
    if (!list) return NULL;

    list->capacity = data_len == 0 ? 1 : data_len;
    list->pairs = malloc(list->capacity * sizeof(RLEPair));
    if (!list->pairs) {
        free(list);
        return NULL;
    }

    list->length = 0;

    if (data_len == 0) return list;

    int current_count = 1;
    int current_value = data[0];

    for (size_t i = 1; i < data_len; i++) {
        if (data[i] == current_value) {
            if (current_count < 9) {
                current_count++;
            } else {
                list->pairs[list->length].count = 9;
                list->pairs[list->length].value = current_value;
                list->length++;
                current_count = 1;
            }
        } else {
            list->pairs[list->length].count = current_count;
            list->pairs[list->length].value = current_value;
            list->length++;
            current_value = data[i];
            current_count = 1;
        }
    }

    list->pairs[list->length].count = current_count;
    list->pairs[list->length].value = current_value;
    list->length++;

    return list;
}

int* rle_decode(RLEList *list, size_t *out_len) {
    if (!list || !list->pairs) {
        *out_len = 0;
        return NULL;
    }

    size_t total_len = 0;
    for (size_t i = 0; i < list->length; i++) {
        total_len += list->pairs[i].count;
    }

    if (total_len == 0) {
        *out_len = 0;
        return calloc(1, sizeof(int));
    }

    int *data = malloc(total_len * sizeof(int));
    if (!data) {
        *out_len = 0;
        return NULL;
    }

    size_t pos = 0;
    for (size_t i = 0; i < list->length; i++) {
        for (int j = 0; j < list->pairs[i].count; j++) {
            data[pos++] = list->pairs[i].value;
        }
    }

    *out_len = total_len;
    return data;
}

void free_rle_list(RLEList *list) {
    if (list) {
        free(list->pairs);
        free(list);
    }
}

int main(void) {
    int data[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3};
    size_t data_len = sizeof(data) / sizeof(data[0]);

    RLEList *encoded = rle_encode(data, data_len);
    if (!encoded) {
        return 1;
    }

    size_t decoded_len;
    int *decoded = rle_decode(encoded, &decoded_len);
    if (!decoded) {
        free_rle_list(encoded);
        return 1;
    }

    for (size_t i = 0; i < encoded->length; i++) {
        printf("(%d, %d) ", encoded->pairs[i].count, encoded->pairs[i].value);
    }
    printf("\n");

    for (size_t i = 0; i < decoded_len; i++) {
        printf("%d ", decoded[i]);
    }
    printf("\n");

    free(decoded);
    free_rle_list(encoded);

    return 0;
}