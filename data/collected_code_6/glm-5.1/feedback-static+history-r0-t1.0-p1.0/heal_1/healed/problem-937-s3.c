#include <stdio.h>
#include <limits.h>

int count_most_common_char(const char *str, size_t max_len) {
    if (str == NULL) {
        return 0;
    }

    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    
    if (len == 0) {
        return 0;
    }

    int counts[UCHAR_MAX + 1] = {0};

    for (size_t i = 0; i < len; i++) {
        unsigned char c = str[i];
        counts[c]++;
    }

    int max_count = 0;
    for (int i = 0; i <= UCHAR_MAX; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
        }
    }

    return max_count;
}

int main(void) {
    const char *text = "hello world";
    size_t max_len = 12;
    int result = count_most_common_char(text, max_len);
    printf("%d\n", result);
    return 0;
}