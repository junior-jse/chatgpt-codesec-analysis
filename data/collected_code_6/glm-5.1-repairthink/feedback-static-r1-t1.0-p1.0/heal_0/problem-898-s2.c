#include <stdio.h>
#include <stdlib.h>

int* extract_continuous(const int* list, size_t list_size, size_t start_index, size_t count, size_t* out_size) {
    if (list == NULL || out_size == NULL) {
        return NULL;
    }

    *out_size = 0;

    if (start_index >= list_size) {
        return NULL;
    }

    size_t available = list_size - start_index;
    if (count > available) {
        return NULL;
    }

    int* result = (int*)malloc(count * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        result[i] = list[start_index + i];
    }

    *out_size = count;
    return result;
}

int main(void) {
    int list[] = {10, 20, 30, 40, 50, 60, 70};
    size_t list_size = sizeof(list) / sizeof(list[0]);
    size_t start_index = 2;
    size_t count = 3;
    size_t out_size = 0;

    int* extracted = extract_continuous(list, list_size, start_index, count, &out_size);

    if (extracted != NULL) {
        for (size_t i = 0; i < out_size; ++i) {
            printf("%d ", extracted[i]);
        }
        printf("\n");
        free(extracted);
    } else {
        printf("Extraction failed.\n");
    }

    return 0;
}