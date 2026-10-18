#include <stdio.h>
#include <limits.h>

int count_most_common_char(const char *str, size_t len) {
    if (str == NULL || len == 0) {
        return 0;
    }

    int counts[UCHAR_MAX + 1] = {0};

    for (size_t i = 0; i < len; i++) {
        counts[(unsigned char)str[i]]++;
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
    int result = count_most_common_char(text, sizeof("hello world") - 1);
    printf("%d\n", result);
    return 0;
}