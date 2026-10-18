#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_duplicate(const char *words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

int remove_duplicate_words(const char *input[], int input_count, const char *output[], int max_output) {
    int output_count = 0;
    for (int i = 0; i < input_count; i++) {
        if (input[i] == NULL) {
            continue;
        }
        if (output_count >= max_output) {
            break;
        }
        if (!is_duplicate(output, output_count, input[i])) {
            output[output_count] = input[i];
            output_count++;
        }
    }
    return output_count;
}

int main(void) {
    const char *input[] = {"apple", "banana", "apple", "cherry", "banana", "date", "apple"};
    int input_count = sizeof(input) / sizeof(input[0]);
    const char **output = malloc(input_count * sizeof(const char *));
    if (output == NULL) {
        return 1;
    }
    int output_count = remove_duplicate_words(input, input_count, output, input_count);
    for (int i = 0; i < output_count; i++) {
        printf("%s\n", output[i]);
    }
    free(output);
    return 0;
}