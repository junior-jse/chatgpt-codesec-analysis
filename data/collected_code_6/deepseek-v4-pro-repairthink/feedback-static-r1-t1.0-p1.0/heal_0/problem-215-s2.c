#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntList;

static int resize_list(IntList *list) {
    size_t new_capacity = list->capacity == 0 ? 16 : list->capacity * 2;
    int *new_data = (int *)realloc(list->data, new_capacity * sizeof(int));
    if (new_data == NULL) {
        return -1;
    }
    list->data = new_data;
    list->capacity = new_capacity;
    return 0;
}

static int append_to_list(IntList *list, int value) {
    if (list->size == list->capacity) {
        if (resize_list(list) != 0) {
            return -1;
        }
    }
    list->data[list->size++] = value;
    return 0;
}

static int decode_rle(const int *encoded, size_t encoded_size, IntList *decoded) {
    if (encoded == NULL || decoded == NULL) {
        return -1;
    }

    for (size_t i = 0; i < encoded_size; i += 2) {
        if (i + 1 >= encoded_size) {
            free(decoded->data);
            decoded->data = NULL;
            decoded->size = 0;
            decoded->capacity = 0;
            return -1;
        }

        int count = encoded[i];
        int value = encoded[i + 1];

        if (count <= 0) {
            free(decoded->data);
            decoded->data = NULL;
            decoded->size = 0;
            decoded->capacity = 0;
            return -1;
        }

        for (int j = 0; j < count; j++) {
            if (append_to_list(decoded, value) != 0) {
                free(decoded->data);
                decoded->data = NULL;
                decoded->size = 0;
                decoded->capacity = 0;
                return -1;
            }
        }
    }

    return 0;
}

static void free_int_list(IntList *list) {
    if (list != NULL) {
        free(list->data);
        list->data = NULL;
        list->size = 0;
        list->capacity = 0;
    }
}

int main(void) {
    int encoded_data[] = {3, 5, 2, 9, 1, 4};
    size_t encoded_size = sizeof(encoded_data) / sizeof(encoded_data[0]);

    IntList decoded = {0};
    decoded.data = NULL;
    decoded.size = 0;
    decoded.capacity = 0;

    if (decode_rle(encoded_data, encoded_size, &decoded) != 0) {
        fprintf(stderr, "Failed to decode RLE data\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < decoded.size; i++) {
        printf("%d ", decoded.data[i]);
    }
    printf("\n");

    free_int_list(&decoded);
    return EXIT_SUCCESS;
}