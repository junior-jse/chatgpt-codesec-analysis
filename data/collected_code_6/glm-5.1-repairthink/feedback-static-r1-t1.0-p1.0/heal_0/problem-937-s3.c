#include <stdio.h>
#include <string.h>
#include <limits.h>

int count_most_common_char(const char *str) {
    if (str == NULL) {
        return 0;
    }

    size_t len = strlen(str);
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
    int result = count_most_common_char(text);
    printf("%d\n", result);
    return 0;
}