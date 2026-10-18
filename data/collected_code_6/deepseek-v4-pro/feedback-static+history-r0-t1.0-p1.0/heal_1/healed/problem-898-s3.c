#include <stdio.h>
#include <stdlib.h>

int extract_continuous_elements(const int *input, int input_size, int *output, int output_size, int start_index) {
    if (input == NULL || output == NULL || input_size <= 0 || output_size <= 0) {
        return -1;
    }

    if (start_index < 0 || start_index >= input_size) {
        return -1;
    }

    if (start_index + output_size > input_size) {
        return -1;
    }

    for (int i = 0; i < output_size; i++) {
        output[i] = input[start_index + i];
    }

    return 0;
}

int main(void) {
    int input_list[] = {10, 20, 30, 40, 50, 60, 70, 80};
    int input_size = sizeof(input_list) / sizeof(input_list[0]);
    int output_size = 4;
    int *output_list = malloc(sizeof(int) * output_size);
    
    if (output_list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    int result = extract_continuous_elements(input_list, input_size, output_list, output_size, 2);
    
    if (result == 0) {
        printf("Extracted elements: ");
        for (int i = 0; i < output_size; i++) {
            printf("%d ", output_list[i]);
        }
        printf("\n");
    } else {
        printf("Extraction failed\n");
    }

    free(output_list);
    output_list = NULL;

    return 0;
}