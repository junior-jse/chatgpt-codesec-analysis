#include <stdio.h>
#include <string.h>
#include <limits.h>

char find_max_occurrence(const char *str, size_t *max_count)
{
    unsigned int counts[UCHAR_MAX + 1] = {0};
    size_t i;
    unsigned char max_char = 0;
    unsigned int max = 0;

    if (str == NULL || *str == '\0') {
        if (max_count != NULL) {
            *max_count = 0;
        }
        return '\0';
    }

    for (i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        counts[c]++;
        if (counts[c] > max) {
            max = counts[c];
            max_char = c;
        }
    }

    if (max_count != NULL) {
        *max_count = (size_t)max;
    }

    return (char)max_char;
}

int main(void)
{
    char buffer[1024];
    size_t count;
    char result;

    printf("Enter a string: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    result = find_max_occurrence(buffer, &count);

    if (count > 0) {
        printf("Maximum occurring character: '%c' (appears %zu times)\n", result, count);
    } else {
        printf("String is empty\n");
    }

    return 0;
}