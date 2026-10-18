#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int calculate_min_length(const char *str) {
    size_t len = strlen(str);
    if (len == 0) {
        return 0;
    }

    size_t total_ops = 0;
    for (size_t i = 0; i < len; ) {
        size_t j = i + 1;
        while (j < len && str[j] == str[i]) {
            j++;
        }
        total_ops++;
        i = j;
    }

    return (int)total_ops;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    const char *input = argv[1];
    int result = calculate_min_length(input);
    printf("%d\n", result);

    return 0;
}