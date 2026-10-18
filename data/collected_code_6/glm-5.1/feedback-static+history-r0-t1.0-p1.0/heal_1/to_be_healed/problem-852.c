#include <stdio.h>
#include <stdlib.h>

/* Possible weaknesses found:
 *  Parameter 'src' can be declared as pointer to const [constParameterPointer]
 */
int remove_negatives(int *src, size_t src_len, int **dst, size_t *dst_len) {
    if (src == NULL || dst == NULL || dst_len == NULL) {
        return -1;
    }

    size_t count = 0;
    for (size_t i = 0; i < src_len; i++) {
        if (src[i] >= 0) {
            count++;
        }
    }

    if (count == 0) {
        *dst = NULL;
        *dst_len = 0;
        return 0;
    }

    int *res = (int *)malloc(count * sizeof(int));
    if (res == NULL) {
        return -1;
    }

    size_t j = 0;
    for (size_t i = 0; i < src_len; i++) {
        if (src[i] >= 0) {
            res[j] = src[i];
            j++;
        }
    }

    *dst = res;
    *dst_len = count;
    return 0;
}

int main(void) {
    int list[] = {5, -2, 0, 8, -1, -7, 3};
    size_t list_len = sizeof(list) / sizeof(list[0]);

    int *filtered = NULL;
    size_t filtered_len = 0;

    if (remove_negatives(list, list_len, &filtered, &filtered_len) == 0) {
        for (size_t i = 0; i < filtered_len; i++) {
            printf("%d ", filtered[i]);
        }
        printf("\n");
        free(filtered);
    }

    return 0;
}