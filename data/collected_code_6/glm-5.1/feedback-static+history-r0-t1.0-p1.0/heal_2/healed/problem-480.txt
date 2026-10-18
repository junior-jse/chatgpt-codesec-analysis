#include <stdio.h>
#include <string.h>

char find_max_occurring_char(const char *str, size_t len) {
    if (str == NULL || len == 0) {
        return '\0';
    }

    int count[256] = {0};

    for (size_t i = 0; i < len; i++) {
        unsigned char c = str[i];
        count[c]++;
    }

    int max_count = -1;
    char max_char = '\0';

    for (size_t i = 0; i < len; i++) {
        unsigned char c = str[i];
        if (count[c] > max_count) {
            max_count = count[c];
            max_char = str[i];
        }
    }

    return max_char;
}

int main(void) {
    const char *str = "experience";
    size_t len = strnlen(str, 1024);
    char result = find_max_occurring_char(str, len);

    if (result != '\0') {
        printf("Maximum occurring character: %c\n", result);
    } else {
        printf("String is empty or NULL.\n");
    }

    return 0;
}