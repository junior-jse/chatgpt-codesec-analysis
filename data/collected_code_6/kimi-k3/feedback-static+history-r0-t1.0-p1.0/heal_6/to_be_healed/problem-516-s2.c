/* Repair history (previous round):
 * - Flawfinder memcpy was removed from radix_sort() and now appears in perform_counting_pass().
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>

#define RADIX 256
#define BITS_PER_BYTE 8

static int validate_array_size(size_t n)
{
    if (n == 0 || n > SIZE_MAX / sizeof(int)) {
        return -1;
    }
    return 0;
}

static int allocate_buffers(size_t n, int **output, int **count)
{
    if (n > SIZE_MAX / sizeof(int)) {
        return -1;
    }
    
    *output = (int *)malloc(n * sizeof(int));
    if (*output == NULL) {
        return -1;
    }
    
    *count = (int *)calloc(RADIX, sizeof(int));
    if (*count == NULL) {
        free(*output);
        *output = NULL;
        return -1;
    }
    
    return 0;
}

static unsigned int find_max_value(const int *arr, size_t n)
{
    unsigned int max_val = (unsigned int)arr[0];
    size_t i;
    
    for (i = 1; i < n; i++) {
        if ((unsigned int)arr[i] > max_val) {
            max_val = (unsigned int)arr[i];
        }
    }
    
    return max_val;
}

static int calculate_passes(unsigned int max_val)
{
    int passes = 0;
    
    while (max_val > 0) {
        passes++;
        max_val >>= BITS_PER_BYTE;
    }
    
    if (passes == 0) {
        passes = 1;
    }
    
    return passes;
}

static int perform_counting_pass(int *arr, size_t n, int *output, int *count, int shift)
{
    size_t i;
    
    memset(count, 0, RADIX * sizeof(int));
    
    for (i = 0; i < n; i++) {
        count[((unsigned int)arr[i] >> shift) & (RADIX - 1)]++;
    }
    
    for (i = 1; i < RADIX; i++) {
        count[i] += count[i - 1];
    }
    
    for (i = n; i > 0; i--) {
        unsigned int key = ((unsigned int)arr[i - 1] >> shift) & (RADIX - 1);
        output[count[key] - 1] = arr[i - 1];
        count[key]--;
    }
    
    if (n > SIZE_MAX / sizeof(int)) {
        return -1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    if (memcpy(arr, output, n * sizeof(int)) == NULL) {
        return -1;
    }
    
    return 0;
}

int radix_sort(int *arr, size_t n)
{
    int *output = NULL;
    int *count = NULL;
    unsigned int max_val;
    int passes;
    int p;
    int result = 0;
    
    if (arr == NULL || validate_array_size(n) != 0) {
        return -1;
    }
    
    if (allocate_buffers(n, &output, &count) != 0) {
        return -1;
    }
    
    max_val = find_max_value(arr, n);
    passes = calculate_passes(max_val);
    
    for (p = 0; p < passes; p++) {
        int shift = p * BITS_PER_BYTE;
        
        if (perform_counting_pass(arr, n, output, count, shift) != 0) {
            result = -1;
            break;
        }
    }
    
    free(output);
    free(count);
    
    return result;
}

int main(void)
{
    int arr[] = {170, 45, 75, 90, 802, 24, 2, 66};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    size_t i;
    int result;
    
    printf("Original array: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    result = radix_sort(arr, n);
    if (result != 0) {
        fprintf(stderr, "Error: radix_sort failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Sorted array: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}