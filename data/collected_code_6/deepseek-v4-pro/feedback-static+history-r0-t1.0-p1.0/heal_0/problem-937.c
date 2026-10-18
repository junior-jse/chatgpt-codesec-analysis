#include <stdio.h>
#include <string.h>
#include <limits.h>

char most_common_char(const char *str) {
    int freq[UCHAR_MAX + 1] = {0};
    int max_freq = 0;
    char most_common = '\0';
    const unsigned char *ustr = (const unsigned char *)str;

    if (str == NULL) {
        return '\0';
    }

    for (size_t i = 0; ustr[i] != '\0'; i++) {
        freq[ustr[i]]++;
        if (freq[ustr[i]] > max_freq) {
            max_freq = freq[ustr[i]];
            most_common = (char)ustr[i];
        }
    }

    return most_common;
}

int main(void) {
    const char *test_str = "hello world";
    char result = most_common_char(test_str);
    printf("Most common character: %c\n", result);
    return 0;
}